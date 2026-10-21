/*
 * A075878  Sum of coefficients of
 *          (x1)^(2i(1))*(x2)^(2i(2))*(x3)^(2i(3))*(x4)^(2i(4)) for
 *          {(i1),(i2),(i3),(i4)}=0,1,2,... : sum(i)=2n in the expansion of
 *          (x1+x2+x3+x4)^(2n) where n >= 1.
 * https://oeis.org/A075878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(4*n-3) + 2^(2*n-1) (OEIS A075878)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 according to the sequence definition */
    idx_t e1 = 4 * n - 3;   /* exponent for the first term */
    idx_t e2 = 2 * n - 1;   /* exponent for the second term */

    /* compute 2^e1 and 2^e2 using left shift on seq_t */
    seq_t t1 = ((seq_t)1 << e1);
    seq_t t2 = ((seq_t)1 << e2);

    return t1 + t2;
}
