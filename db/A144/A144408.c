/*
 * A144408  Last digit of A135266(n).
 * https://oeis.org/A144408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence: a(n + 12) = a(n).  The first 12 terms are
   0, 1, 5, 9, 0, 2, 6, 9, 9, 1, 6, 0. */
#include <stddef.h>

static const seq_t _a144408_pattern[12] = {
    (seq_t)0, (seq_t)1, (seq_t)5, (seq_t)9, (seq_t)0, (seq_t)2,
    (seq_t)6, (seq_t)9, (seq_t)9, (seq_t)1, (seq_t)6, (seq_t)0
};

seq_t a(idx_t n)
{
    idx_t r = n % 12;
    return _a144408_pattern[r];
}
