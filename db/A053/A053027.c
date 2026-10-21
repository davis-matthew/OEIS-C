/*
 * A053027  Odd primes p with 2 zeros in Fibonacci numbers mod p.
 * https://oeis.org/A053027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth odd prime p such that the Pisano period π(p)
 *            equals twice the rank of apparition r(p) (the smallest positive
 *            n with p|F_n)
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"

/* --------------------------------------------------------------- */
/*  Fast doubling Fibonacci modulo mod                              */
/* --------------------------------------------------------------- */
static void fib_mod_pair(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    uint64_t a, b, c, d;
    fib_mod_pair(n >> 1, mod, &a, &b);    /* a=F_k , b=F_{k+1}   */
    /* c = F_{2k}   = a * (2*b - a)   */
    /* d = F_{2k+1} = a*a + b*b      */
    uint64_t t = (2 * b) % mod;
    if (t < a) t += mod;            /* avoid negative mod          */
    t = (t + mod - a) % mod;        /* 2*b - a (mod mod)          */
    c = ((unsigned __int128)a * t) % mod;
    d = ((unsigned __int128)a * a + (unsigned __int128)b * b) % mod;

    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d) % mod;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

/* --------------------------------------------------------------- */
/*  Generate all divisors of a factorized number                    */
/* --------------------------------------------------------------- */
static void gen_divs_rec(uint64_t idx, uint64_t cur,
                         const uint64_t *pfac, const int *expo, int fac_cnt,
                         uint64_t *divs, size_t *divcnt)
{
    if (idx == (uint64_t)fac_cnt) {
        divs[(*divcnt)++] = cur;
        return;
    }
    uint64_t prod = 1;
    for (int e = 0; e <= expo[idx]; ++e) {
        gen_divs_rec(idx + 1, cur * prod, pfac, expo, fac_cnt,
                     divs, divcnt);
        prod *= pfac[idx];
    }
}

/* Comparator for qsort on uint64_t array */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* --------------------------------------------------------------- */
/*  Check whether a prime p satisfies π(p) = 2 · r(p)               */
/* --------------------------------------------------------------- */
static bool prime_has_two_zeros(uint64_t p, const uint64_t *prime_pool,
                                size_t prime_pool_cnt)
{
    if (p == 2 || p == 5) return false;

    /* Compute N = p-1 or 2*(p+1) according to p mod 5 */
    uint64_t N;
    uint64_t r = p % 5;
    if (r == 1 || r == 4)
        N = p - 1;
    else
        N = 2 * (p + 1);

    /* Factorize N using the prime pool (sufficient for N ≤ 2·10⁶) */
    uint64_t tmp = N;
    uint64_t pfac[20];
    int expo[20];
    int fac_cnt = 0;
    for (size_t i = 0; i < prime_pool_cnt &&
         (uint64_t)prime_pool[i] * (uint64_t)prime_pool[i] <= tmp;
         ++i) {
        uint64_t q = prime_pool[i];
        if (tmp % q == 0) {
            int e = 0;
            while (tmp % q == 0) {
                tmp /= q;
                ++e;
            }
            pfac[fac_cnt] = q;
            expo[fac_cnt] = e;
            ++fac_cnt;
        }
    }
    if (tmp > 1) {
        pfac[fac_cnt] = tmp;
        expo[fac_cnt] = 1;
        ++fac_cnt;
    }

    /* Generate all divisors of N */
    uint64_t divs[1024];
    size_t divcnt = 0;
    gen_divs_rec(0, 1, pfac, expo, fac_cnt, divs, &divcnt);

    /* Sort the divisors to ensure increasing order */
    qsort(divs, divcnt, sizeof(uint64_t), cmp_uint64);

    /* Find rank r (first divisor with F_d ≡ 0) and period per (first
       divisor with (F_d, F_{d+1}) ≡ (0,1) ) */
    uint64_t rank = 0, period = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divs[i];
        if (d <= 1) continue;                /* skip 1, because F_1 ≠ 0 */
        uint64_t f_n, f_n1;
        fib_mod_pair(d, p, &f_n, &f_n1);
        if (rank == 0 && f_n == 0) rank = d;
        if (period == 0 && f_n == 0 && f_n1 == 1) period = d;
        if (rank && period) break;
    }
    if (rank == 0 || period == 0) return false; /* should not happen */

    return period == 2 * rank;
}

/* --------------------------------------------------------------- */
/*  Main sequence function                                         */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static data shared across calls */
    static bool initialized = false;
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t *prime_list = NULL;
    static size_t prime_list_len = 0;
    static size_t prime_list_next = 0;

    /* Fast initialization: sieve primes up to a safe limit */
    if (!initialized) {
        const idx_t LIMIT = 1500000;                /* enough for n ≤ 10 000 */
        bool *is_comp = (bool *)malloc((LIMIT + 1) * sizeof(bool));
        for (idx_t i = 2; i <= LIMIT; ++i) is_comp[i] = false;
        for (idx_t i = 2; (i * i) <= LIMIT; ++i)
            if (!is_comp[i])
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;

        /* Count primes first */
        for (idx_t i = 2; i <= LIMIT; ++i)
            if (!is_comp[i]) ++prime_list_len;
        prime_list = (uint64_t *)malloc(prime_list_len * sizeof(uint64_t));
        size_t idx = 0;
        for (idx_t i = 2; i <= LIMIT; ++i)
            if (!is_comp[i]) prime_list[idx++] = (uint64_t)i;

        free(is_comp);

        /* Initialise terms storage */
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));

        initialized = true;
    }

    /* Generate more terms until we reach the requested index */
    while (terms_len < (size_t)n) {
        if (prime_list_next >= prime_list_len) break; /* safety check */

        uint64_t p = prime_list[prime_list_next++];
        if (!prime_has_two_zeros(p, prime_list, prime_list_len))
            continue;

        append_candidate(&terms, &terms_len, &terms_cap, (seq_t)p);
    }

    /* Return the requested term (1‑based index) */
    return terms[(size_t)(n - 1)];
}
