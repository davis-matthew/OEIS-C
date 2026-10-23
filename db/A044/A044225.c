/*
 * A044225  Numbers k such that string 4,6 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer k such that the octal representation of
 *            k contains the substring "46" but the octal representation of
 *            k-1 does not
 * Range:     u8 n <= 4, u16 n <= 1136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has46_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has46_oct.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_start = 1;            /* next k to test */
    /* initialise the cache */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    /* extend cache until we have n terms */
    while (cache_len < n) {
        seq_t k = next_start;
        bool hk   = has46_oct(k);
        bool hk1  = has46_oct(k - (seq_t)1);
        if (hk && !hk1) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = k;
        }
        next_start = k + (seq_t)1;
    }
    return cache[n - 1];
}
