/*
 * A016758  a(n) = (2*n+1)^6.
 * https://oeis.org/A016758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)^6
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 19, u64 n <= 812,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)2 * (seq_t)n + (seq_t)1;
    seq_t result = base;
    for (int i = 1; i < 6; ++i)
        result *= base;
    return result;
}
