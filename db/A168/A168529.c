/*
 * A168529  Primes n that are concatenation of two 3-digit primes p<q.
 * https://oeis.org/A168529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1407, u64 n <= 1407, u128 n <= 1407,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth prime of the form 10^3 * p + q with 101 ≤ p < q ≤ 997, 
   where p and q are both prime.  The function builds a static sorted 
   cache of all such primes on its first call.  The cache size is 10203,
   well below 2^16. */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdbool.h>  /* bool, false, true */
#include <stdlib.h>   /* malloc, qsort */

/* Comparison function for qsort: works for integer and arbitrary‑precision
   types that provide operator< in C++ or normal < in C. */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* primality test for numbers up to 1e6 using a precomputed list of primes
   up to 1000.  The list is passed as an array of uint64_t and its size. */
static bool is_prime_candidate(uint64_t cand,
                               const uint64_t *primes,
                               size_t nprimes)
{
    for (size_t i = 0; i < nprimes; ++i) {
        uint64_t p = primes[i];
        if (p * p > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* sequence function                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache variables ------------------------------------------------ */
    static seq_t *cache = NULL;   /* sorted array of all primes */
    static size_t cache_len = 0;  /* number of cached elements */

    /* build cache on first call ------------------------------------------- */
    if (cache == NULL) {
        /* 1. sieve primes up to 1000 */
        bool comp[1001] = { false };
        uint64_t sieve_primes[200];
        size_t sieve_cnt = 0;

        for (uint64_t i = 2; i <= 1000; ++i) {
            if (!comp[i]) {
                if (sieve_cnt < sizeof(sieve_primes) / sizeof(sieve_primes[0]))
                    sieve_primes[sieve_cnt++] = i;
                if (i * i <= 1000)
                    for (uint64_t j = i * i; j <= 1000; j += i)
                        comp[j] = true;
            }
        }

        /* 2. extract primes between 101 and 997 inclusive */
        uint64_t primes[200];
        size_t p_cnt = 0;
        for (size_t i = 0; i < sieve_cnt; ++i) {
            uint64_t p = sieve_primes[i];
            if (p >= 101 && p <= 997)
                primes[p_cnt++] = p;
        }

        /* 3. generate all candidates and test primality */
        const size_t MAX_PAIRS = 11000;          /* exceeds 143*142/2 = 10203 */
        cache = (seq_t *)malloc(sizeof(seq_t) * MAX_PAIRS);
        cache_len = 0;

        for (size_t i = 0; i < p_cnt; ++i) {
            uint64_t p = primes[i];
            for (size_t j = i + 1; j < p_cnt; ++j) {
                uint64_t q = primes[j];
                uint64_t cand = p * 1000ULL + q; /* n = 10^3 * p + q */
                if (is_prime_candidate(cand, sieve_primes, sieve_cnt))
                    cache[cache_len++] = (seq_t)cand;
            }
        }

        /* 4. sort the cache */
        qsort(cache, cache_len, sizeof(seq_t), seq_cmp);
    }

    /* --------------------------------------------------------------------- */
    /* answer the query                                                      */
    /* --------------------------------------------------------------------- */
    if (n == 0 || (idx_t)n > cache_len)
        return 0;        /* n out of range – behaviour undefined after offset */

    return cache[(size_t)(n - 1)];
}
