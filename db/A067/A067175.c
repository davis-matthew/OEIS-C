/*
 * A067175  Number of digits in the n-th primorial (A002110).
 * https://oeis.org/A067175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of decimal digits of the product of the * first n
 *            primes (the n‑th primorial). * a(0) = 1. For n>0, a(n) =
 *            ⌊∑_{k=1..n} log10(p_k)⌋ + 1, * where p_k is the k‑th prime. This
 *            implementation caches * intermediate results so that calls may
 *            be made in any order
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

seq_t
a(idx_t n)
{
    /* Cached data for all previously computed n */
    static seq_t      *digit_cache = NULL;   /* a(i) for 0 <= i <= processed_n */
    static size_t     cache_cap   = 0;
    static long double *log_sum   = NULL;    /* prefix sums of log10(primes) */
    static size_t      log_cap     = 0;
    static uint64_t    *prime_arr  = NULL;   /* primes[0..prime_cnt-1]        */
    static size_t      prime_cnt   = 0;
    static idx_t       processed_n = 0;       /* highest n already computed   */
    static bool        initialized = false;   /* true after first init */

    /* Ensure capacity for digit_cache and log_sum */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        digit_cache = (seq_t *)realloc(digit_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    if (log_cap <= (size_t)n) {
        size_t new_cap = log_cap ? log_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        log_sum = (long double *)realloc(log_sum, new_cap * sizeof(long double));
        log_cap = new_cap;
    }

    /* First‑time initialization */
    if (!initialized) {
        digit_cache[0] = 1;         /* a(0) = 1 (empty primorial)   */
        log_sum[0] = 0.0L;          /* log10(1) = 0                   */
        processed_n = 0;
        initialized = true;
    }

    /* Return cached value if known */
    if (n <= processed_n)
        return digit_cache[(size_t)n];

    /* Ensure we have at least n primes.  Use Rosser's bound to estimate
     * an upper limit for the n‑th prime. */
    if (prime_cnt < (size_t)n) {
        idx_t need_n = (idx_t)n;
        double limit;
        if (need_n < 6) {
            limit = 15.0;
        } else {
            double dn = (double)need_n;
            limit = dn * (log(dn) + log(log(dn))) + 3.0;
        }
        idx_t max_limit = (idx_t)(limit) + 1;

        uint64_t *tmp_primes = NULL;
        size_t    tmp_cnt    = 0;
        build_primes_upto(max_limit, &tmp_primes, &tmp_cnt);

        size_t new_cnt = tmp_cnt;
        if (new_cnt > (size_t)n) new_cnt = (size_t)n;

        free(prime_arr);
        prime_arr = (uint64_t *)malloc(new_cnt * sizeof(uint64_t));
        for (size_t i = 0; i < new_cnt; ++i)
            prime_arr[i] = tmp_primes[i];

        prime_cnt = new_cnt;
        free(tmp_primes);
    }

    /* Compute values for new indices. */
    for (idx_t i = processed_n; i < n; ++i) {
        uint64_t  prime = prime_arr[(size_t)i];
        long double add  = log10((long double)prime);
        log_sum[(size_t)(i + 1)] = log_sum[(size_t)i] + add;

        /* digits = floor(log_sum) + 1; cast through unsigned long long to
         * avoid issues with very large `long double` values. */
        unsigned long long d =
            (unsigned long long)floor(log_sum[(size_t)(i + 1)]) + 1ULL;
        digit_cache[(size_t)(i + 1)] = (seq_t)d;
    }

    processed_n = n;
    return digit_cache[(size_t)n];
}
