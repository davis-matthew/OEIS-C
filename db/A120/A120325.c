/*
 * A120325  Period 6: repeat [0, 0, 1, 0, 1, 0].
 * https://oeis.org/A120325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = period-6 pattern [0, 0, 1, 0, 1, 0]
 * Range:     u8 n <= 9910, u16 n <= 9910, u32 n <= 9910, u64 n <= 9910,
 *            u128 n <= 9910, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {0, 0, 1, 0, 1, 0};
    return pattern[n % 6];
}
