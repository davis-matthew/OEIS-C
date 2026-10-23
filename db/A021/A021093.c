/*
 * A021093  Decimal expansion of 1/89.
 * https://oeis.org/A021093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/89) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    int rem = 1;        /* remainder after integer part of 1/89 */
    int digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;                /* bring next decimal digit */
        digit = rem / 89;         /* integer part of the division */
        rem %= 89;                /* new remainder for next step */
    }
    return (seq_t)digit;
}
