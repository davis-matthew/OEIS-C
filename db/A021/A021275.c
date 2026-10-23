/*
 * A021275  Decimal expansion of 1/271.
 * https://oeis.org/A021275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * (10^n mod 271) / 271); the digits of 1/271
 *            repeat with period 5: 0,0,3,6,9
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t digits[5] = {0,0,3,6,9};
    return digits[n % 5];
}
