/*
 * A016756  a(n) = (2*n+1)^4.
 * https://oeis.org/A016756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)^4
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 127, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t   = (seq_t)n;
    seq_t odd = (seq_t)2 * t + (seq_t)1;
    seq_t sq  = odd * odd;
    return sq * sq;
}
