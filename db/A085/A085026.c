/*
 * A085026  a(n) = (6*n+1)*(6*n+7).
 * https://oeis.org/A085026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n+1)*(6*n+7)
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the sequence type before any arithmetic */
    seq_t m = (seq_t)n;
    seq_t t1 = 6 * m + 1;
    seq_t t2 = 6 * m + 7;
    return t1 * t2;
}
