/*
 * A210021  Number of binary words of length n containing no subword 11011.
 * https://oeis.org/A210021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n for n < 5; a(n) = 2*a(n-1) - a(n-3) + a(n-4) + a(n-5)
 *            for n ≥ 5
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 66, u128 n <= 133,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Ensure cache has entries up to index n */
    if ((size_t)n + 1 > cache_size) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure */
        cache = tmp;

        for (size_t i = cache_size; i < new_cap; ++i) {
            if (i < 5)
                cache[i] = pow2((idx_t)i);
            else
                cache[i] = 2 * cache[i-1]
                           - cache[i-3]
                           + cache[i-4]
                           + cache[i-5];
        }
        cache_size = new_cap;
    }

    return cache[(size_t)n];
}
