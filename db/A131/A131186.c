/*
 * A131186  Period 12: repeat [0, 1, 2, 0, 2, 4, 0, 4, 3, 0, 3, 1].
 * https://oeis.org/A131186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n mod 12), where the period 12 cycle is [0, 1, 2, 0, 2,
 *            4, 0, 4, 3, 0, 3, 1]
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* precomputed period of length 12 */
    static const seq_t pat[12] = { 0, 1, 2, 0, 2, 4,
                                   0, 4, 3, 0, 3, 1 };
    return pat[n % 12];
}
