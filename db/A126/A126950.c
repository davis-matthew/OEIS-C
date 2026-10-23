/*
 * A126950  a(1) = 1; for n>1, a(n) = the smallest number p > a(n-1) such that
 *          (a(n-1)+p)/2 is a cube.
 * https://oeis.org/A126950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n +1)*(2*n^2 + 2*n -1)+ 5*(-1)^n)/4
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = (seq_t)2 * nn + (seq_t)1;
    seq_t t2 = (seq_t)2 * nn * nn + (seq_t)2 * nn - (seq_t)1;
    seq_t prod = t1 * t2;
    if ((n & (idx_t)1) == 0) /* n is even */
        prod += (seq_t)5;
    else                      /* n is odd */
        prod -= (seq_t)5;
    return prod / (seq_t)4;
}
