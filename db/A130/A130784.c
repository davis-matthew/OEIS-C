/*
 * A130784  Period 3: repeat [1, 3, 2].
 * https://oeis.org/A130784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 - ((n+5) % 3)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t mod = (n + 5) % 3;
    return (seq_t)(3 - mod);
}
