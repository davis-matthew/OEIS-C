/*
 * A016779  a(n) = (3*n + 1)^3.
 * https://oeis.org/A016779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + 1)^3
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 541, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert to seq_t to keep all arithmetic in the right type */
    seq_t nn = (seq_t)n;
    seq_t x  = nn * (seq_t)3 + (seq_t)1;
    seq_t xx = x * x;
    return xx * x;
}
