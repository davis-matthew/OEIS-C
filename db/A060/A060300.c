/*
 * A060300  a(n) = (2*n*(n+1))^2.
 * https://oeis.org/A060300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(n+1))^2
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 180, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* convert index to the sequence type */
    seq_t t = x * (x + 1);       /* n*(n+1) */
    t = t * (seq_t)2;            /* 2*n*(n+1) */
    t = t * t;                   /* square the result */
    return t;
}
