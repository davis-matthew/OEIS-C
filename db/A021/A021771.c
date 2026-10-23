/*
 * A021771  Decimal expansion of 1/767.
 * https://oeis.org/A021771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth digit after the decimal point in the decimal
 *            expansion of 1/767
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t den = (seq_t)767;     /* divisor */
    seq_t rem = (seq_t)1;             /* current remainder */
    seq_t digit = 0;
    idx_t i;

    for (i = 0; i <= n; ++i) {
        rem *= (seq_t)10;            /* bring next digit into place */
        digit = rem / den;           /* next decimal digit */
        rem   = rem % den;           /* update remainder */
    }
    return digit;
}
