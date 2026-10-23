/*
 * A016982  a(n) = (7*n)^2.
 * https://oeis.org/A016982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n)^2
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * (seq_t)7;
    return t * t;
}
