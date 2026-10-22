/*
 * A006198  Number of partitions into pairs.
 * https://oeis.org/A006198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   (2n-3)*a(n) = (4n^2-8n+1)*a(n-1) + (2n-1)*a(n-2), a(1) = 1, a(2)
 *            = 1. For fixed-width seq_t, the division by the odd number 2n-3
 *            is replaced by multiplication by its inverse mod 2^w. This gives
 *            the exact value whenever it fits, even if intermediate products
 *            overflow
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

static seq_t inv_odd(seq_t c) {
    /* Newton iteration for the inverse of an odd c modulo 2^w */
    seq_t x = c;
    int k;
    for (k = 0; k < 7; k++) x = x * (2 - c * x);
    return x;
}

static idx_t cur_n = 0;
static seq_t cur_a = 0;   /* a(cur_n) */
static seq_t prev_a = 0;  /* a(cur_n - 1) */

seq_t a(idx_t n) {
    bool fixed;
    if (n < 1) return 0;
    if (n <= 2) return 1;
    fixed = seq_is_fixed_width();
    if (cur_n < 2 || n < cur_n) {
        cur_n = 2;
        cur_a = 1;
        prev_a = 1;
    }
    while (cur_n < n) {
        idx_t i = cur_n + 1;
        seq_t s = (seq_t)i;
        seq_t c1 = 4 * s * s - 8 * s + 1;
        seq_t c2 = 2 * s - 1;
        seq_t c3 = 2 * s - 3;
        seq_t num = c1 * cur_a + c2 * prev_a;
        seq_t next;
        if (fixed)
            next = num * inv_odd(c3);
        else
            next = num / c3;
        prev_a = cur_a;
        cur_a = next;
        cur_n = i;
    }
    return cur_a;
}
