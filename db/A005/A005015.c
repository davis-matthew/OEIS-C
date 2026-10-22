/*
 * A005015  a(n) = 11*2^n.
 * https://oeis.org/A005015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*2^n
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)11 << n;
}
