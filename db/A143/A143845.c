/*
 * A143845  Period 8: repeat [0, 8, 12, 14, 15, 7, 3, 1].
 * https://oeis.org/A143845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9^n mod 17) - 1
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is periodic with period 8:
       0, 8, 12, 14, 15, 7, 3, 1. */
    static const seq_t period[8] = {0, 8, 12, 14, 15, 7, 3, 1};
    return period[n % 8];
}
