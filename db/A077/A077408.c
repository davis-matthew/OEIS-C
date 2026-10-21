/*
 * A077408  Trajectory of 103 under the Reverse and Add! operation carried out
 *          in base 3, written in base 10.
 * https://oeis.org/A077408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 103 a(n+1) = a(n) + reverse_base3(a(n)), where
 *            reverse_base3(x) is obtained by writing x in base‑3, reversing
 *            the digit order, and interpreting the result as a base‑10 number
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   reverse_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_add.h"

/* Main sequence function with memoisation. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)103;    /* a(0) */
    }

    /* Grow cache up to the requested index. */
    while (cache_size <= n) {
        seq_t val = reverse_add(cache[cache_size - 1]);
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (cache_size + 1) * sizeof(seq_t));
        if (!new_cache) {          /* allocation failure: keep old cache */
            break;
        }
        cache = new_cache;
        cache[cache_size] = val;
        ++cache_size;
    }

    return cache[(size_t)n];
}
