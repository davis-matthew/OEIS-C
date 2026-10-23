/*
 * A043405  Numbers whose base-7 representation contains exactly one 3.
 * https://oeis.org/A043405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer whose base‑7 representation contains exactly
 *            one digit ‘3’
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   count3base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/count3base7.h"

/* Static cache for memoisation. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_used = 0;
static seq_t current_val = 1;

/* Grow the cache to hold at least 'needed' elements. */
static void grow_cache(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *ptr = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
    if (!ptr && new_cap > 0)
        exit(1); /* Allocation failure */
    cache = ptr;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* Not defined for n=0, return 0 */

    if (n > cache_used) {
        grow_cache(n);
        while (cache_used < n) {
            if (count3base7(current_val) == 1)
                cache[cache_used++] = current_val;
            current_val++;
        }
    }
    return cache[n-1];
}
