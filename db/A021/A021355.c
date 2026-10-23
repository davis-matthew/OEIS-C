/*
 * A021355  Decimal expansion of 1/351.
 * https://oeis.org/A021355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th digit after the decimal point in the decimal
 *            expansion of 1/351
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digits repeat with the period 0,0,2,8,4,9, so a(n) = pattern[n mod 6] */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {0, 0, 2, 8, 4, 9};
    return pattern[n % 6];
}
