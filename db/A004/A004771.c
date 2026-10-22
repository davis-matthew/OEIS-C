/*
 * A004771  a(n) = 8*n + 7. Or, numbers whose binary expansion ends in 111.
 * https://oeis.org/A004771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n + 7
 * Range:     u8 n <= 31, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * 8 + 7;
}
