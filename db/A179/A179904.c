/*
 * A179904  a(n) = A056520(n)+1 for n>0, a(0)=1.
 * https://oeis.org/A179904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3+3*n^2+n+12)/6 for n>0, a(0)=1
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1631, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t t = (seq_t)2 * n;  /* 2n */
    t += (seq_t)3;           /* 2n + 3 */
    t *= n;                  /* (2n + 3) * n */
    t += (seq_t)1;           /* + 1 */
    t *= n;                  /* * n */
    t += (seq_t)12;          /* + 12 */
    return t / (seq_t)6;
}
