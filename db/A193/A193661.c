/*
 * A193661  Q-residue of the triangle A193673, where Q is the triangular array
 *          (t(i,j)) given by t(i,j)=1. (See Comments.)
 * https://oeis.org/A193661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 31*a(n-2) + 21*a(n-3) with a(0)=1, a(1)=3,
 *            a(2)=15
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)15;

    /* compute iteratively up to n */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)3;   /* a(n-2) */
    seq_t a2 = (seq_t)15;  /* a(n-1) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        /* ai = 11*a2 + 21*a0 - 31*a1  (safe order to avoid unsigned underflow) */
        seq_t temp = ((seq_t)11 * a2) + ((seq_t)21 * a0);
        ai = temp - ((seq_t)31 * a1);

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return a2;
}
