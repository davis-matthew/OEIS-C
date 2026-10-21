/*
 * A228138  Number of blocks in a Steiner system S(2, 4, A228137(n+1)).
 * https://oeis.org/A228138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*floor(n/2)^2 + floor(n/2) if n is even 12*floor(n/2)^2
 *            + 7*floor(n/2) + 1 if n is odd a(1) = 4
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)4;

    idx_t k = n >> 1;          /* floor(n/2) */
    seq_t kk = (seq_t)k;

    if ((n & 1) == 0)          /* n even */
        return (12 * kk * kk + kk);
    else                       /* n odd */
        return (12 * kk * kk + 7 * kk + 1);
}
