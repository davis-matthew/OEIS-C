/*
 * A186424  Odd terms in A186423.
 * https://oeis.org/A186424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(1+2*n+2*n^2)/4 + (-1)^n*(1+2*n)/4
 * Range:     u8 n <= 5, u16 n <= 160, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute the numerator of the fraction */
    seq_t numerator = (seq_t)3 + (seq_t)6 * nn + (seq_t)6 * nn * nn;
    if ((n & 1) == 0)
        numerator += (seq_t)1 + (seq_t)2 * nn;   /* (+1+2n) for even n */
    else
        numerator -= (seq_t)1 + (seq_t)2 * nn;   /* -(1+2n) for odd n  */
    return numerator / (seq_t)4;
}
