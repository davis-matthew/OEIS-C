/*
 * A225869  Limiting sequence of a counting procedure.
 * https://oeis.org/A225869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the periodic sequence with fundamental period 2, 2, 0,
 *            0, 2, 2, 0, 1, 1, 1, 1, 0, 3, 1, 0, 0, 1 (offset 1: a(1)=2).
 *            Hence a(n) = period[(n-1) mod 17]
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[17] = {
        (seq_t)2, (seq_t)2, (seq_t)0, (seq_t)0, (seq_t)2, (seq_t)2, (seq_t)0,
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)0, (seq_t)3, (seq_t)1,
        (seq_t)0, (seq_t)0, (seq_t)1
    };
    /* Compute index modulo 17 safely. */
    idx_t idx = (n - 1) % 17;
    return period[(size_t)idx];
}
