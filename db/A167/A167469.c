/*
 * A167469  a(n) = 3*n*(5*n-1)/2.
 * https://oeis.org/A167469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(5*n-1)/2
 * Range:     u8 n <= 4, u16 n <= 68, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n=0 but safe to return 0 */
        return 0;
    seq_t nn = (seq_t)n;
    seq_t t  = 5 * nn - 1;      /* 5*n-1 */
    seq_t tmp = 3 * nn * t;     /* 3*n*(5*n-1) */
    return tmp / 2;             /* divide by 2 */
}
