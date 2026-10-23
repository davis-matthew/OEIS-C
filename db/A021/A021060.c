/*
 * A021060  Decimal expansion of 1/56.
 * https://oeis.org/A021060
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

/*  OEIS A021060: Decimal expansion of 1/56
    Formula: a(n) is the nth digit after the decimal point of 1/56.  */

seq_t a(idx_t n)
{
    seq_t rem   = 1;   /* current remainder of 1/56 after processing n digits */
    seq_t digit = 0;   /* digit produced in the current step */

    for (idx_t i = 0; i <= n; ++i) {
        rem   *= 10;          /* bring next decimal digit into place   */
        digit  = rem / 56;    /* integer part is the current decimal digit */
        rem   %= 56;          /* remainder for the next iteration      */
    }
    return digit;
}
