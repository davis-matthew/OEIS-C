/*
 * A016983  a(n) = (7*n)^3.
 * https://oeis.org/A016983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n)^3
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 232, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t mul = (seq_t)n * (seq_t)7;
    return mul * mul * mul;
}
