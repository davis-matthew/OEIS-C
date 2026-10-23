/*
 * A126761  {a(k)} is such that, for every positive integer n, the n-th prime
 *          = Sum_{k=1..n, gcd(k,n+1)=1} a(k).
 * https://oeis.org/A126761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   p_n = Σ_{k=1..n, gcd(k,n+1)=1} a(k) where p_n is the n‑th prime
 *            (p_1 = 2). The function computes a(n) with memoisation
 * Range:     i8 n <= 40, i16 n <= 169, i32 n <= 531, i64 n <= 1207,
 *            i128 n <= 2715, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* External integer types supplied by the build system.
   Do not redefine them. */

/* Memoisation tables – 1‑based indexing. */
static seq_t *a_cache   = NULL; /* a(k) for k = 1..cache_cap            */
static seq_t *sum_mult  = NULL; /* sum_mult[d] = Σ_{k≤cache_cap, d|k} a(k) */
static idx_t cache_cap = 0;     /* current capacity of the arrays        */

/* Smallest prime factor array for factorisation. */
static idx_t *spf = NULL;       /* spf[n] = smallest prime factor of n   */
static idx_t spf_cap = 0;       /* maximum n for which spf is ready       */

/* List of primes – 0‑based indexing. */
static uint32_t *prime_list = NULL;
static idx_t prime_cnt = 0;

/* Largest index that has been processed. */
static idx_t num_comp = 0;

/* ---- Utility functions -------------------------------------------------- */

/* Ensure the cache arrays are large enough for indices up to 'need'. */
static void extend_cache(idx_t need)
{
    if (cache_cap >= need) return;
    idx_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < need) new_cap <<= 1;

    a_cache  = (seq_t *)realloc(a_cache,  (new_cap + 1) * sizeof(seq_t));
    sum_mult = (seq_t *)realloc(sum_mult, (new_cap + 1) * sizeof(seq_t));
    if (!a_cache || !sum_mult) exit(EXIT_FAILURE);
    for (idx_t i = cache_cap + 1; i <= new_cap; ++i) {
        a_cache[i]  = 0;
        sum_mult[i] = 0;
    }
    cache_cap = new_cap;
}

/* Build smallest‑prime‑factor table up to 'limit'. */
static void ensure_spf(idx_t limit)
{
    if (spf_cap >= limit) return;
    free(spf);
    spf = (idx_t *)calloc(limit + 1, sizeof(idx_t));
    if (!spf) exit(EXIT_FAILURE);
    spf_cap = limit;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

/* Grow the prime list so that at least 'n_needed' primes are stored. */
static void ensure_primes(idx_t n_needed)
{
    if (prime_cnt >= n_needed) return;

    double dn = (double)n_needed;
    int bound;
    if (n_needed < 6) bound = 15;
    else bound = (int)(dn * (log(dn) + log(log(dn))) + 10);
    if (bound < 20) bound = 20;

    bool *is_prime = (bool *)calloc(bound + 1, sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);
    memset(is_prime, 1, bound + 1);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i <= bound; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= bound; j += i)
                is_prime[j] = false;

    free(prime_list);
    prime_list = (uint32_t *)malloc((bound + 1) * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);
    prime_cnt = 0;
    for (int i = 2; i <= bound && (size_t)prime_cnt < n_needed; ++i)
        if (is_prime[i]) prime_list[prime_cnt++] = (uint32_t)i;

    free(is_prime);
}

/* Factor x into its distinct prime factors; return count. */
static size_t factor_distinct(idx_t x, idx_t pk[])
{
    size_t cnt = 0;
    while (x > 1) {
        idx_t p = spf[x];
        pk[cnt++] = p;
        while (x % p == 0) x /= p;
    }
    return cnt;
}

/* Generate all divisors of n (including 1 and n). Result stored in 'divs',
   and *cnt is the number of divisors. */
static void gen_divisors(idx_t n, idx_t divs[], size_t *cnt)
{
    idx_t primes[32];
    int exps[32];
    int pcnt = 0;
    while (n > 1) {
        idx_t p = spf[n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        primes[pcnt] = p;
        exps[pcnt]   = e;
        ++pcnt;
    }

    *cnt = 1;
    divs[0] = 1;
    for (int i = 0; i < pcnt; ++i) {
        size_t cur = *cnt;
        idx_t p = primes[i];
        idx_t mult = 1;
        for (int e = 1; e <= exps[i]; ++e) {
            mult *= p;
            for (size_t j = 0; j < cur; ++j) {
                divs[*cnt] = divs[j] * mult;
                ++(*cnt);
            }
        }
    }
}

/* ---- Main sequence function --------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined but safe */

    /* Ensure all auxiliary structures are large enough. */
    if (n + 1 > cache_cap) extend_cache(n + 1);
    if (n + 1 > spf_cap)   ensure_spf(n + 1);
    ensure_primes(n);      /* need at least n primes */

    /* Return memoised value if already computed. */
    if (n <= num_comp) return a_cache[n];

    /* Compute values sequentially up to n. */
    for (idx_t k = num_comp + 1; k <= n; ++k) {
        idx_t m = k + 1;                /* n+1 of the defining equation */
        seq_t sum_all = sum_mult[1];    /* Σ_{i<k} a(i) */

        /* Distinct prime factors of m. */
        idx_t pk[32];
        size_t np = factor_distinct(m, pk);

        /* Inclusion–exclusion over subsets of these primes. */
        seq_t sum_gcd1 = sum_all;
        size_t subsets = (1UL << np);
        for (size_t mask = 1; mask < subsets; ++mask) {
            idx_t prod = 1;
            int bits = 0;
            for (size_t j = 0; j < np; ++j) {
                if (mask & (1UL << j)) {
                    prod *= pk[j];
                    ++bits;
                }
            }
            if (prod <= cache_cap) {
                if (bits & 1)
                    sum_gcd1 -= sum_mult[prod];
                else
                    sum_gcd1 += sum_mult[prod];
            }
        }

        /* k‑th prime (0‑based index). */
        seq_t pk_val = (seq_t)prime_list[k - 1];

        /* a(k) = p_k – Σ_{i<k, gcd(i,m)=1} a(i) */
        seq_t a_k = pk_val - sum_gcd1;
        a_cache[k] = a_k;

        /* Update sum_mult for all divisors of k. */
        idx_t div_buf[256];
        size_t dcnt;
        gen_divisors(k, div_buf, &dcnt);
        for (size_t idx = 0; idx < dcnt; ++idx) {
            idx_t d = div_buf[idx];
            sum_mult[d] += a_k;
        }
    }

    num_comp = n;
    return a_cache[n];
}
