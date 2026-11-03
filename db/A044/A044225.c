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
 * Validated: standard, pass, 57 terms through n = 9784 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* helper: returns true iff the octal representation of x contains
   the substring 46 */
static bool has46_oct(seq_t x) {
    if (x < (seq_t)8) return false;  /* need at least two digits */
    int digits[64];
    int len = 0;
    seq_t t = x;
    while (t > 0) {
        digits[len++] = (int)(t % (seq_t)8);
        t /= (seq_t)8;
    }
    /* digits are stored LSB first; check consecutive pairs */
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 4 && digits[i - 1] == 6) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_start = 1;            /* next k to test */
    /* initialise the cache */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }
    /* extend cache until we have n terms */
    while (cache_len < n) {
        seq_t k = next_start;
        bool hk   = has46_oct(k);
        bool hk1  = has46_oct(k - (seq_t)1);
        if (hk && !hk1) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = k;
        }
        next_start = k + (seq_t)1;
    }
    return cache[n - 1];
}
