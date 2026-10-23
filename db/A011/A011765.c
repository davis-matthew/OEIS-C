/*
 * A011765  Period 4: repeat [0, 0, 0, 1].
 * https://oeis.org/A011765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n ≡ 0 (mod 4), otherwise 0 (period 4 pattern [0, 0,
 *            0, 1])
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)((n & 3) == 0);
}
