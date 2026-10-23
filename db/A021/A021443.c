/*
 * A021443  Decimal expansion of 1/439.
 * https://oeis.org/A021443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/439 (starting from
 *            n = 0; a(0) is the first digit after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t rem   = 1;           /* current remainder in 1/439 long division */
    idx_t digit = 0;           /* hold the digit for the current position */

    for (idx_t i = 0; i <= n; ++i)
    {
        idx_t tmp  = rem * 10;     /* multiply remainder by 10 */
        digit      = tmp / 439;    /* new decimal digit */
        rem        = tmp % 439;    /* new remainder */
    }

    return (seq_t)digit;            /* return the requested digit */
}
