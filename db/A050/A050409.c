/*
 * A050409  Truncated square pyramid numbers: a(n) = Sum_{k = n..2*n} k^2.
 * https://oeis.org/A050409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)*(14*n+1)/6
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 674, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS A050409, the sum of squares from k=n to k=2n. */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t t1 = sn * (sn + 1);            /* n*(n+1)   */
    seq_t t2 = (seq_t)(14 * n) + 1;      /* 14*n + 1   */
    seq_t t  = t1 * t2;                  /* product    */
    t /= 6;                               /* exact division */
    return t;
}
