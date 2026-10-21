/*
 * A079524  Expansion of (x + b*x^2 - b*x^3)/((1 - x^2)*(1 - x)^2) with b=2.
 * https://oeis.org/A079524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n*(n+6)-3*(1-(-1)^n))/8, a(0)=0, a(1)=1,
 * Range:     u8 n <= 8, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                       /* convert index to seq_t */
    seq_t tmp = 2 * nn * (nn + 6);              /* 2*n*(n+6)  */
    seq_t odd_part = (n & 1) ? 6 : 0;           /* 3*(1-(-1)^n) : 0 if even, 6 if odd */
    tmp -= odd_part;                            /* subtract the odd part */
    return tmp >> 3;                            /* divide by 8 (exact) */
}
