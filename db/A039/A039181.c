/*
 * A039181  Numbers whose base-11 representation has the same number of 2's
 *          and 10's.
 * https://oeis.org/A039181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   has_equal_2_and_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039181: numbers whose base-11 representation has the same number of 2's and 10's */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_2_and_10.h"

/* static state for memoization */
static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static size_t seq_cache_len = 0;
static idx_t seq_next = 0;

/* grow the cache array if needed */
static void ensure_cache_cap(size_t needed) {
    if (needed <= seq_cache_cap) return;
    size_t new_cap = needed * 2 + 64;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) /* undefined in OEIS, return 0 to avoid crash */
        return (seq_t)0;
    if ((size_t)n <= seq_cache_len)
        return seq_cache[n - 1];
    /* generate terms until we reach the nth one */
    while (seq_cache_len < (size_t)n) {
        if (has_equal_2_and_10(seq_next)) {
            ensure_cache_cap(seq_cache_len + 1);
            seq_cache[seq_cache_len++] = (seq_t)seq_next;
        }
        ++seq_next;
    }
    return seq_cache[n - 1];
}
