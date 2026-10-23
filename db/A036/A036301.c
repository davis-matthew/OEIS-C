/*
 * A036301  Numbers whose sum of even digits and sum of odd digits are equal.
 * https://oeis.org/A036301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2569, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   balanced
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose sum of even digits and sum of odd digits are equal.
   This function returns the nth such number with a(1) = 0. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/balanced.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity          */
    static seq_t last_val   = 0;   /* last value found            */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1024;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;  /* a(1) = 0 */
        cache_len = 1;
        last_val = (seq_t)0;
    }

    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    /* index in cache for the answer */
    idx_t idx = n - 1;
    if (idx < (idx_t)cache_len)
        return cache[idx];

    /* extend cache until it contains the required element */
    seq_t v = last_val;
    while ((idx_t)cache_len <= idx) {
        ++v;
        if (balanced(v)) {
            if (cache_len == cache_cap) {          /* grow */
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = v;
            last_val = v;
        }
    }
    return cache[idx];
}
