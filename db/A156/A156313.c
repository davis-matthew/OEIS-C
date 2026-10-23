/*
 * A156313  Primes that cannot be written as prime(k) + prime(k+1)*m.
 * https://oeis.org/A156313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes that cannot be written as prime(k) + prime(k+1)*m
 *            for any integers k ≥ 1 and m ≥ 1. This algorithm generates all
 *            primes up to an increasing limit, marks which primes can be
 *            expressed as such, collects the unrepresentable ones, and
 *            memoises the result. It is efficient enough for n up...
 * Range:     u8 n <= 12, u16 n <= 539, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Build a prime table up to a given limit.  The returned array must be
   freed by the caller. */
#include "../utils/build_primes.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Refill the cached sequence until it contains at least 'target' terms. */
static void recompute_up_to(idx_t target)
{
    idx_t limit = 100;                    /* initial bound */

    for (;;) {
        /* ---- 1) Generate primes up to 'limit'. ---- */
        idx_t *primes_raw = NULL;
        size_t prime_cnt = 0;
        build_primes(limit, &primes_raw, &prime_cnt);

        /* ---- 2) Mark which numbers ≤ limit are primes. ---- */
        size_t arr_size = (size_t)limit + 1;
        bool *is_prime = (bool *)calloc(arr_size, sizeof(bool));
        for (size_t i = 0; i < prime_cnt; ++i)
            is_prime[(size_t)primes_raw[i]] = true;

        /* ---- 3) Mark primes that have a representation. ---- */
        bool *rep = (bool *)calloc(arr_size, sizeof(bool));
        for (size_t i = 0; i + 1 < prime_cnt; ++i) {
            idx_t pk  = primes_raw[i];
            idx_t pk1 = primes_raw[i + 1];
            idx_t val = pk + pk1;          /* m = 1 */
            while (val <= limit) {
                if (is_prime[(size_t)val])
                    rep[(size_t)val] = true;
                val += pk1;
            }
        }

        /* ---- 4) Collect primes that cannot be represented. ---- */
        seq_t *new_cache = (seq_t *)calloc(prime_cnt, sizeof(seq_t));
        size_t new_len = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = primes_raw[i];
            if (!rep[(size_t)p])
                new_cache[new_len++] = (seq_t)p;
        }

        /* ---- 5) Replace the global cache. ---- */
        free(cache);
        cache     = new_cache;
        cache_len = new_len;

        /* ---- 6) Clean up temporary data. ---- */
        free(primes_raw);
        free(is_prime);
        free(rep);

        /* ---- 7) Stop when we have enough terms. ---- */
        if (cache_len >= (size_t)target)
            return;

        limit *= 2;                    /* increase bound and repeat */
    }
}

/* Public function used by the evaluator.  The OEIS offset is 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for offset 0 */

    if ((size_t)n > cache_len)     /* need more terms */
        recompute_up_to(n);

    return cache[(size_t)n - 1];
}
