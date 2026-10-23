/*
 * A152179  Period 9: repeat [8, 2, 7, 5, 5, 7, 2, 8, 7].
 * https://oeis.org/A152179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 2*n + 8) mod 9
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 9 first to avoid overflow. */
    idx_t m = n % 9;
    idx_t val = (m * m + 2 * m + 8) % 9;
    return (seq_t)val;
}
