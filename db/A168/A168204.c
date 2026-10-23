/*
 * A168204  a(n) = 4*n - a(n-1) + 1 with n>1, a(1)=1.
 * https://oeis.org/A168204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 3 + 5*(-1)^n)/2
 * Range:     u8 n <= 125, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (n & 1)          /* n is odd */
        return 2*nn - 1;
    else                /* n is even */
        return 2*nn + 4;
}
