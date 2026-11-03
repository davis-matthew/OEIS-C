/*
 * A044748  Numbers n such that string 3,5 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer n such that the decimal representation
 *            of n contains the substring "35" but that of n+1 does not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool contains35(seq_t x) {
    int digits[32];
    int len = 0;
    while (x > (seq_t)0) {
        digits[len++] = (int)(x % (seq_t)10);
        x /= (seq_t)10;
    }
    if (len < 2) return false;
    for (int i = len - 1; i >= 1; --i) {
        if (digits[i] == 3 && digits[i - 1] == 5)
            return true;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* Undefined for the offset case, but safe. */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static idx_t next_to_test = 35; /* Start at the first possible term. */

    if (cache == NULL) {                      /* First call initialisation */
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
        cache_size = 0;
    }

    /* Extend cache until we have at least n terms */
    while (cache_size < (size_t)n) {
        seq_t val = (seq_t)next_to_test;
        if (contains35(val) && !contains35(val + (seq_t)1)) {
            if (cache_size == cache_cap) { /* Grow cache */
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_size++] = val;
        }
        ++next_to_test;
    }

    return cache[n - 1];
}
