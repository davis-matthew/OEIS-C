/*
 * A021255  Decimal expansion of 1/251.
 * https://oeis.org/A021255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (10^(n+1) / 251) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth digit after the decimal point in the decimal expansion of 1/251. */
seq_t a(idx_t n)
{
    /* The remainder after the decimal point for digits of 1/251. */
    idx_t rem = 1;                     /* start from 1/251 */

    /* Compute successive decimal digits by long division. */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;                     /* bring down next digit 0 */
        seq_t digit = (seq_t)(rem / 251); /* quotient */
        rem = rem % 251;               /* new remainder */
        if (i == n)
            return digit;              /* nth digit found */
    }
    return (seq_t)0;                    /* never reached, keeps compiler happy */
}
