/*
 * A213661  Number of dominating subsets of the wheel graph W_n.
 * https://oeis.org/A213661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + 2^(n-4) for n >= 4 Initial
 *            values: a(1) = 4, a(2) = 3, a(3) = 7
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    if (n < 1) {
        return 0;   /* outside the defined range */
    }

    /* Ensure the cache can hold index n */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;          /* indices 0 .. n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {                      /* allocation failed */
            return 0;
        }
        cache = tmp;
        size_t old_cap = cap;
        cap = new_cap;
        size_t start = old_cap;                  /* first index to compute */

        for (size_t i = start; i < cap; ++i) {
            if (i == 0) {
                /* index 0 is unused, leave it zero-initialized */
                continue;
            }
            if (i == 1) {
                cache[i] = (seq_t)4;
            } else if (i == 2) {
                cache[i] = (seq_t)3;
            } else if (i == 3) {
                cache[i] = (seq_t)7;
            } else { /* i >= 4 */
                cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                           + pow2((idx_t)(i-4));
            }
        }
    }

    return cache[(size_t)n];
}
