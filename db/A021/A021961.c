/*
 * A021961  Decimal expansion of 1/957.
 * https://oeis.org/A021961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = ⌊10^(n+1) / 957⌋ mod 10
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rem = 1;   /* remainder of the division 1*10^k by 957 */
    seq_t digit = 0; /* the digit at position n */

    /* Long division: compute successive decimal digits of 1/957 */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;          /* shift one decimal place */
        digit = rem / 957;  /* quotient gives the current digit */
        rem   = rem % 957;  /* remainder for next step */
    }
    return digit;          /* digit is in [0,9] */
}
