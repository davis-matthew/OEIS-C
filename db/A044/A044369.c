/*
 * A044369  Numbers n such that string 3,7 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_37
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for n such that decimal representation contains substring "37"
   but (n-1) does not contain it. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains_37.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t tested = 0;   /* last integer examined */

    if (n == 0)
        return 0;   /* undefined, but safe for out-of-bounds */

    /* Grow cache to hold at least n values */
    if (cache_len < n) {
        if (cache_cap == 0) {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }
        while (cache_len < n) {
            idx_t x = tested + 1;
            ++tested;

            if (contains_37(x) && !contains_37(x - 1)) {
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)x;
            }
        }
    }
    return cache[n - 1];
}
