/*
 * A047555  Numbers that are congruent to {0, 1, 2, 6, 7} mod 8.
 * https://oeis.org/A047555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * floor((n-1)/5) + [0,1,2,6,7][(n-1)%5] (offset 1)
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t _offsets[5] = { (seq_t)0, (seq_t)1, (seq_t)2,
                                   (seq_t)6, (seq_t)7 };

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 0. For n < 1 the behaviour is irrelevant. */
    if (n == 0) return 0;

    idx_t q = (n - 1) / 5;          /* number of full 8‑block groups */
    idx_t r = (n - 1) % 5;          /* position inside the current block */

    /* cast q to seq_t before multiplication to keep all arithmetic in
       seq_t for the result. This is safe because seq_t is at least 8 bits
       wide (see build assumptions). */
    seq_t val = (seq_t)8 * (seq_t)q + _offsets[r];
    return val;
}
