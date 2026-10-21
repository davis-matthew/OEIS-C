/*
 * A061927  a(n) = n*(n+1)*(2*n+1)*(n^2+n+3)/30.
 * https://oeis.org/A061927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n+1)*(n^2+n+3)/30
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 143, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;
    seq_t t1 = nt * (nt + (seq_t)1);           /* n(n+1) */
    seq_t t2 = ((seq_t)2) * nt + (seq_t)1;     /* 2n+1 */
    seq_t t3 = nt * nt + nt + (seq_t)3;        /* n²+n+3 */
    seq_t prod = t1 * t2;                      /* (n(n+1))(2n+1) */
    prod = prod * t3;                          /* * (n²+n+3) */
    return prod / (seq_t)30;                   /* divide by 30 */
}
