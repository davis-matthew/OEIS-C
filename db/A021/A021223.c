/*
 * A021223  Decimal expansion of 1/219.
 * https://oeis.org/A021223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic decimal digits of 1/219 (period length 8) */
seq_t a(idx_t n)
{
    static const seq_t digits[8] = {0, 0, 4, 5, 6, 6, 2, 1};
    return digits[n & 7];
}
