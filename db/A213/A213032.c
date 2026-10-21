/*
 * A213032  a(n) = floor(n/2)*floor(n/3)-floor(n/6)^2.
 * https://oeis.org/A213032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2)*floor(n/3) - floor(n/6)^2
 * Range:     u8 n <= 43, u16 n <= 686, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t2 = (seq_t)(n / 2);
    seq_t t3 = (seq_t)(n / 3);
    seq_t t6 = (seq_t)(n / 6);
    return t2 * t3 - t6 * t6;
}
