/*
 * A027480  a(n) = n*(n+1)*(n+2)/2.
 * https://oeis.org/A027480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = n*(n+1)*(n+2)/2
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x * (x + 1) * (x + 2) / 2;
}
