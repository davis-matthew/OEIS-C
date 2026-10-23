/*
 * A021538  Decimal expansion of 1/534.
 * https://oeis.org/A021538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit after the decimal point in the decimal
 *            expansion of 1/534. a(n) = ( floor(10^(n+1) / 534) ) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t digit = 0;
    unsigned int rem = 1;          /* remainder after dividing 1 by 534 */
    for (idx_t i = 0; i <= n; ++i) {
        unsigned int temp = rem * 10U;   /* multiply by 10; fits in unsigned int */
        digit = (seq_t)(temp / 534U);    /* next decimal digit */
        rem   = temp % 534U;             /* update remainder for next step */
    }
    return digit;
}
