/*
 * A016876  a(n) = (5*n + 2)^4.
 * https://oeis.org/A016876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 2)^4
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 50, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n * 5 + 2;
    seq_t k2 = k * k;
    return k2 * k2;
}
