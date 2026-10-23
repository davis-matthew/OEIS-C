/*
 * A021339  Decimal expansion of 1/335.
 * https://oeis.org/A021339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th digit after the decimal point in the decimal
 *            expansion of 1/335
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         (a(0) is the first digit, a(1) the second, etc.) */

seq_t a(idx_t n)
{
    /* Use an unsigned 64‑bit integer for the intermediate remainder; it
     * never exceeds 334, so this is always safe regardless of the
     * size of seq_t. */
    idx_t rem    = 1;          /* current remainder of 1 / 335          */
    seq_t digit  = 0;          /* last digit computed, <= 9             */

    /* Each iteration produces the next decimal digit.  We need n+1
     * iterations to reach the digit at index n. */
    for (idx_t i = 0; i <= n; ++i) {
        rem = rem * 10;                 /* shift remainder left by one decimal place */
        digit = rem / (idx_t)335;        /* next digit is the integer part            */
        rem   = rem % (idx_t)335;       /* update remainder for the next iteration   */
    }

    return digit;
}
