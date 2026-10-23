/*
 * A016025  Least k such that (tau(k^k)+k-1)/k=n.
 * https://oeis.org/A016025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 32, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least k such that (tau(k^k)+k-1)/k = n.  The minimal k is
   2^(n-1) unless a smaller composite k < n satisfies the equation,
   in which case the smallest such k is returned. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Local cache for already computed values. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache has room for indices up to n. */
static void ensure_cache(idx_t n)
{
    size_t needed = n + 1;
    if (needed > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) abort();          /* allocation failure */
        /* initialise the new part of the cache to 0 (uncomputed) */
        for (size_t i = cache_cap; i < needed; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cache_cap = needed;
    }
}

/* Return the minimal k for a given n. */
seq_t a(idx_t n)
{
    if (n == 0) /* the sequence is offset 1; n==0 is unused */
        return 0;

    ensure_cache(n);
    if (cache[n] != 0)          /* value already computed? */
        return cache[n];

    seq_t result;

    if (n == 1) {               /* a(1) = 1 */
        result = 1;
    } else {
        bool found = false;

        /* Search for a smaller composite k (must be < n). */
        for (idx_t k = 2; k < n; ++k) {
            /* Compute tau(k^k) = product over prime factors p^e of k
               of (e*k + 1). */
            uint64_t tau = 1;
            idx_t temp = k;

            for (idx_t p = 2; p * p <= temp; ++p) {
                if (temp % p == 0) {
                    idx_t e = 0;
                    while (temp % p == 0) {
                        ++e;
                        temp /= p;
                    }
                    tau *= (e * k + 1);
                }
            }
            if (temp > 1)            /* remaining prime factor */
                tau *= (k + 1);

            uint64_t rhs = (n - 1) * k + 1;

            if (tau == rhs) {
                result = (seq_t)k;
                found = true;
                break;              /* smallest k found */
            }
        }

        if (!found)
            result = pow2(n - 1);   /* default solution 2^(n-1) */
    }

    cache[n] = result;
    return result;
}
