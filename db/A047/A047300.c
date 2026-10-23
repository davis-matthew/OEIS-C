/*
 * A047300  Numbers that are congruent to {2, 3, 4, 6} mod 7.
 * https://oeis.org/A047300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numbers congruent to {2,3,4,6} modulo 7. Recurrence: a(n)
 *            = 7*floor((n-1)/4) + offs[(n-1)%4], where offs = {2,3,4,6}
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t OFFSETS[4] = {2, 3, 4, 6};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset starts at 1; undefined for n==0 */
    idx_t n_minus1 = n - 1;
    idx_t block   = n_minus1 / 4;    /* 0‑based block index */
    idx_t pos     = n_minus1 % 4;    /* position within the block */
    return ((seq_t)block * 7) + OFFSETS[pos];
}
