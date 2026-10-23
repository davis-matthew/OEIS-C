/*
 * A171892  0, 1 and all numbers >= 9.
 * https://oeis.org/A171892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 1, 1 for n = 2, n + 6 for n ≥ 3
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 1;
    return (seq_t)n + (seq_t)6;
}
