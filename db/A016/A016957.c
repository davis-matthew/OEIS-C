/*
 * A016957  a(n) = 6*n + 4.
 * https://oeis.org/A016957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n + 4
 * Range:     u8 n <= 41, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)6 + (seq_t)4;
}
