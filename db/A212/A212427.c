/*
 * A212427  a(n) = 17*n + A000217(n-1).
 * https://oeis.org/A212427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+33)/2
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)         /* a(0) = 0 */
        return 0U;      /* 0 as seq_t */

    seq_t nn  = (seq_t)n;           /* convert index to seq_t */
    seq_t t1  = 17 * nn;            /* 17*n  */
    seq_t t2  = ((nn - 1) * nn) / 2;/* (n-1)*n/2  */
    return t1 + t2;                 /* sum gives n*(n+33)/2 */
}
