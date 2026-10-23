/*
 * A193662  Q-residue of the Lucas triangle A114525, where Q is the triangle
 *          given by t(i,j)=1 for 0<=i<=j. (See Comments.)
 * https://oeis.org/A193662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 4*a(n-3) for n>3, with a(0)=2,
 *            a(1)=1, a(2)=5, a(3)=7
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)7;

    /* Initial values */
    seq_t a1 = (seq_t)1;  /* a(1) */
    seq_t a2 = (seq_t)5;  /* a(2) */
    seq_t a3 = (seq_t)7;  /* a(3) */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)2 * a3 + (seq_t)3 * a2 - (seq_t)4 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
