/*
 * A021969  Decimal expansion of 1/965.
 * https://oeis.org/A021969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the nth digit (0‑based) of the decimal expansion of 1/965
 * Range:     u8 n <= 3, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t divisor = 965;          /* denominator */
    seq_t remainder = (seq_t)1;         /* start with the numerator 1 */
    seq_t digit = 0;

    /* Compute the digits from a(0) up to a(n) */
    for (idx_t i = 0; i <= n; ++i) {
        remainder = remainder * 10;     /* shift left one decimal place */
        digit      = remainder / divisor;
        remainder  = remainder % divisor;
    }

    return digit;
}
