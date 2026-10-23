/*
 * A130974  Period 6: repeat [1, 1, 1, 3, 3, 3].
 * https://oeis.org/A130974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 - (-1)^floor(n/3) (period 6: 1,1,1,3,3,3 ...)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor division by 3 gives the required period index */
    idx_t period_index = n / 3u;
    /* parity of the period index determines the value */
    return (seq_t)(1 + 2 * (period_index & 1u));
}
