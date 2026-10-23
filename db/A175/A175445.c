/*
 * A175445  a(n)>a(n-1), a(n) = smallest prime such that a(n)+a(n-1) is
 *          multiple of m, a(1)=2, m=9.
 * https://oeis.org/A175445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 971, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 2;
 * a(n) = smallest prime > a(n-1) such that (a(n) + a(n-1)) % 9 == 0
 * (OEIS A175445, m = 9)
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[1..len] holds computed terms */
    static size_t cap   = 0;
    static idx_t  len   = 0;      /* number of terms stored, >=0 */

    /* Ensure the base case a(1)=2 exists */
    if (len == 0) {
        cap = 1;
        cache = (seq_t *)realloc(cache, (cap + 1) * sizeof(seq_t));
        if (!cache) abort();           /* handle allocation failure */
        cache[1] = (seq_t)2;
        len = 1;
    }

    /* Grow the cache and compute any missing terms up to n. */
    if (n > len) {
        /* Resize cache */
        size_t new_cap = cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cap   = new_cap;

        /* Compute terms from len+1 up to n */
        for (idx_t i = len + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];

            /* Start with the first integer greater than prev */
            seq_t cand = prev + 1;

            /* Ensure we start from the smallest odd number >2 */
            if (cand <= 2) cand = 2;
            if (cand > 2 && (cand & 1) == 0) cand += 1;

            /* Find smallest prime satisfying the modulo condition */
            while (!(is_prime_big(cand) && (((cand + prev) % 9u) == 0))) {
                /* Increment candidate to the next odd number */
                cand += 1;
                if (cand > 2 && (cand & 1) == 0) cand += 1;
            }

            cache[i] = cand;
        }
        len = n;
    }

    return cache[n];
}
