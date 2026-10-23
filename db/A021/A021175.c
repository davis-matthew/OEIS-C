/*
 * A021175  Decimal expansion of 1/171.
 * https://oeis.org/A021175
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

/* Decimal expansion of 1/171
   a(0) = floor(10/171) % 10
   a(n) = floor(10^(n+1)/171) % 10
   The digits are produced by repeated long division.
*/
seq_t a(idx_t n)
{
    idx_t rem = 1;          /* remainder of the division so far */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= 10;                      /* shift left one decimal place */
        digit = rem / 171;              /* next decimal digit */
        rem   = rem % 171;              /* update remainder */
    }
    return digit;
}
