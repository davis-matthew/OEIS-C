/*
 * A011670  A binary m-sequence: expansion of reciprocal of x^6+x^4+x^3+x+1.
 * https://oeis.org/A011670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑1) XOR a(n‑3) XOR a(n‑4) XOR a(n‑6) over GF(2) * with
 *            initial values a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1. *
 *            * The function caches computed terms so that repeated calls for
 *            arbitrary * indices are answered in O(1) time after an O(max n)
 *            precomputation. * It works for any inte...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure we have space for at least the first six terms. */
    idx_t needed = (n + 1 > 6 ? n + 1 : 6);

    if (n >= cache_size) {
        /* Grow the cache. */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)needed * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; terminate program (cannot recover). */
            exit(1);
        }
        cache = tmp;

        /* If this is the first allocation, initialise the seed. */
        if (cache_size == 0) {
            cache[0] = 0;
            cache[1] = 0;
            cache[2] = 0;
            cache[3] = 0;
            cache[4] = 0;
            cache[5] = 1;
            cache_size = 6;
        }

        /* Compute missing terms up to index n. */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i - 1] ^ cache[i - 3] ^ cache[i - 4] ^ cache[i - 6];
        }
        cache_size = needed;
    }

    return cache[n];
}
