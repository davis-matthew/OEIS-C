/*
 * A154519  a(n) = 216*n + 12.
 * https://oeis.org/A154519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 216*n + 12
 * Range:     u8 n <= 1, u16 n <= 303, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * 216 + 12;
}
