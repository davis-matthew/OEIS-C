/*
 * A143615  Triangle A054521 * A000005 as a vector.
 * https://oeis.org/A143615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n, gcd(n,m)=1} tau(m) (A143615)
 * Range:     u8 n <= 60, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global tables for divisor counts (tau) and their prefix sums */
static seq_t *tau_arr = NULL;      /* 1‑based: tau_arr[i] = τ(i) */
static seq_t *prefix_arr = NULL;   /* prefix_arr[i] = Σ_{k=1..i} τ(k) */
static idx_t  tau_size = 0;        /* current allocated size (max index + 1) */

/* Ensure tables up to at least N are filled */
static void ensure_divisor_tables(idx_t N)
{
    if (N < 1) return;
    if (tau_arr && tau_size > N) return;   /* already computed up to N */

    idx_t new_size = N + 1;                 /* 1‑based indices */
    size_t sz = (size_t)new_size;
    seq_t *new_tau = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *new_prefix = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!new_tau || !new_prefix) {
        free(new_tau); free(new_prefix);
        return; /* allocation failure */
    }

    /* initialize */
    for (size_t i = 0; i < sz; ++i) {
        new_tau[i] = 0;
        new_prefix[i] = 0;
    }

    /* build τ array */
    for (idx_t i = 1; i <= N; ++i)
        for (idx_t j = i; j <= N; j += i)
            new_tau[j]++;

    /* build prefix sums */
    for (idx_t i = 1; i <= N; ++i)
        new_prefix[i] = new_prefix[i-1] + new_tau[i];

    /* swap in the new tables */
    free(tau_arr);
    free(prefix_arr);
    tau_arr = new_tau;
    prefix_arr = new_prefix;
    tau_size = new_size;
}

/* Add distinct prime factors of n into primes[0..] and return count */
static uint8_t factor_unique_primes(idx_t n, uint64_t primes[64])
{
    uint8_t cnt = 0;
    if (n % 2 == 0) {
        primes[cnt++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    for (idx_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            primes[cnt++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) primes[cnt++] = n;
    return cnt;
}

/* Cache of previously computed a(n) values */
static seq_t *a_cache = NULL;
static idx_t  a_cache_size = 0;

static void ensure_a_cache(idx_t N)
{
    if (N < a_cache_size) return;
    size_t new_sz = (size_t)(N + 1);
    seq_t *new_cache = (seq_t*)realloc(a_cache, new_sz * sizeof(seq_t));
    if (!new_cache) return; /* allocation failure */
    for (size_t i = a_cache_size; i < new_sz; ++i) new_cache[i] = 0;
    a_cache = new_cache;
    a_cache_size = N + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not defined for n=0 */

    ensure_a_cache(n);
    if (a_cache[n] != 0) return a_cache[n];

    ensure_divisor_tables(n);
    if (!tau_arr || !prefix_arr) return 0; /* allocation failure */

    /* Base sum over all m ≤ n */
    seq_t sumPos = prefix_arr[n];
    seq_t sumNeg = 0;

    /* Distinct prime factors of n */
    uint64_t primes[64];
    uint8_t pcnt = factor_unique_primes(n, primes);

    /* Inclusion‑exclusion over square‑free divisors of n */
    if (pcnt > 0) {
        uint64_t subsetCount = 1ULL << pcnt;   /* 2^pcnt subsets */
        for (uint64_t mask = 1; mask < subsetCount; ++mask) {
            idx_t prod = 1;
            int bits = 0;
            for (uint8_t k = 0; k < pcnt; ++k) {
                if (mask & (1ULL << k)) {
                    idx_t p = (idx_t)primes[k];
                    if (prod > n / p) { prod = n + 1; break; }
                    prod *= p;
                    bits++;
                }
            }
            if (prod > n) continue;  /* no multiples within [1..n] */

            /* Sum τ over multiples of prod */
            seq_t mult_sum = 0;
            for (idx_t t = prod; t <= n; t += prod)
                mult_sum += tau_arr[t];

            if (bits & 1)      /* odd subset – subtract */
                sumNeg += mult_sum;
            else               /* even subset – add back */
                sumPos += mult_sum;
        }
    }

    seq_t result = sumPos - sumNeg;   /* sumPos ≥ sumNeg for all n ≥ 1 */
    a_cache[n] = result;
    return result;
}
