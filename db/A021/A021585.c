/*
 * A021585  Decimal expansion of 1/581.
 * https://oeis.org/A021585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Decimal digits of 1/581
   a(0) = 0            (integer part)
   a(n) = the (n+1)th decimal digit of 1/581, for n ≥ 1
   (the first decimal digit is omitted, matching the OEIS listing)
*/

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* integer part */

    seq_t remainder = 1;          /* start with the numerator */
    seq_t digit     = 0;

    /* Perform (n+1) steps of long division to reach the required digit */
    for (idx_t i = 0; i <= n; ++i)
    {
        remainder *= 10;          /* bring next decimal place into the numerator */
        digit     = remainder / 581;
        remainder %= 581;
    }

    return digit;
}
