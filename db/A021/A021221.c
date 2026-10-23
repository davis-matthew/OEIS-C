/*
 * A021221  Decimal expansion of 1/217.
 * https://oeis.org/A021221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/217) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal digit of the decimal expansion of 1/217 */

seq_t a(idx_t n)
{
    idx_t rem = 1;      /* current remainder of the long division    */
    seq_t digit = 0;    /* digit to be returned                       */

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;                  /* bring next decimal place */
        digit = (seq_t)(rem / 217);  /* the current digit       */
        rem %= 217;                  /* keep only the remainder   */
    }

    return digit;
}
