/*
 * A016757  a(n) = (2*n+1)^5.
 * https://oeis.org/A016757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)^5
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 41, u64 n <= 3565,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)2 * (seq_t)n + 1;
    seq_t result = base;
    for (idx_t i = 1; i < 5; ++i)
        result *= base;
    return result;
}
