/*
 * A021413  Decimal expansion of 1/409.
 * https://oeis.org/A021413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digit of 1/409:  a(n) = floor(10^(n+1)/409) mod 10,
   computed by repeated long division. */

seq_t a(idx_t n)
{
    /* Remainder of 1/409 before multiplying by 10. */
    unsigned int rem = 1;
    seq_t digit = 0;

    /* Compute digits from the first up to the nth digit. */
    for (idx_t i = 0; i <= n; ++i) {
        rem   *= 10;                       /* shift remainder left by one decimal place */
        digit  = (seq_t)(rem / 409);       /* next decimal digit */
        rem    = rem % 409;                /* new remainder */
    }
    return digit;
}
