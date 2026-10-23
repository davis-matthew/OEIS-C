/*
 * A131532  Period 6: repeat [0, 0, 0, 0, 1, 1].
 * https://oeis.org/A131532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   repeat [0, 0, 0, 0, 1, 1] every 6 terms
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 6;
    return (r == 4 || r == 5) ? 1 : 0;
}
