/*
 * A021526  Decimal expansion of 1/522.
 * https://oeis.org/A021526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n-th decimal digit of 1/522 after the decimal point,
 *            computed by long division
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The remainder after each division step fits in idx_t (max 5210). */
    idx_t rem   = 1;   /* initial remainder of 1 / 522 */
    idx_t digit = 0;   /* current decimal digit */

    /* Run the long division step n+1 times to reach the nth digit
       (indices start at 0). */
    for (idx_t i = 0; i <= n; ++i) {
        rem   *= 10;          /* bring down the next digit */
        digit = rem / 522;    /* integer part = next decimal digit */
        rem   %= 522;         /* remainder for the next step */
    }

    return (seq_t)digit;       /* final digit fits in seq_t (0–9) */
}
