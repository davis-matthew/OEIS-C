/*
 * A044731  Numbers n such that string 1,8 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers n whose decimal representation contains the
 *            substring "18" but n+1 does not
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_18
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_18.h"

seq_t a(idx_t n)
{
    /* Static cache for previously found terms */
    static seq_t *cache      = NULL;      /* array of found terms          */
    static size_t cache_sz   = 0;         /* number of terms stored        */
    static size_t cache_cap  = 0;         /* capacity of the array         */
    static seq_t  next_candidate = (seq_t)18;   /* next integer to test          */

    if (n == (idx_t)0) return (seq_t)0;                /* not used (offset 1)   */

    /* Initialise storage on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    size_t target = (size_t)n;
    if (target > cache_sz) {
        /* Ensure we have enough space in the array */
        if (target > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            while (new_cap < target) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Scan integers from the last tested value until we find
           enough terms to reach the requested index. */
        seq_t cur = next_candidate;
        while (cache_sz < target) {
            if (has_18(cur) && !has_18(cur + (seq_t)1))
                cache[cache_sz++] = cur;
            ++cur;
        }
        next_candidate = cur;                      /* update where we left off   */
    }

    return cache[target - 1];
}
