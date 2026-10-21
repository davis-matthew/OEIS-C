/*
 * A213029  a(n) = floor(n/2)^2 - floor(n/3)^2.
 * https://oeis.org/A213029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2)^2 - floor(n/3)^2
 * Range:     u8 n <= 43, u16 n <= 685, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t h = (seq_t)(n / 2);
    seq_t t = (seq_t)(n / 3);
    return h * h - t * t;
}
