/*
 * A182308  a(n+1) = a(n) + floor(a(n)/7) with a(0) = 7.
 * https://oeis.org/A182308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 71, u32 n <= 154, u64 n <= 321,
 *            u128 n <= 653, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 7; a(n+1) = a(n) + floor(a(n)/7) */
#include <stddef.h>

seq_t a(idx_t n) {
    seq_t val = (seq_t)7;          /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        val = val + (val / (seq_t)7);
    }
    return val;
}
