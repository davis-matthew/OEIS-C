/*
 * A133496  a(n) = (29*n)^2.
 * https://oeis.org/A133496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (29*n)^2
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 750, u64 n <= 750, u128 n <= 750,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t = t * (seq_t)29;
    return t * t;
}
