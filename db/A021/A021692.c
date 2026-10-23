/*
 * A021692  Decimal expansion of 1/688.
 * https://oeis.org/A021692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A021692: Decimal digits of 1/688
    Formula: a(n) = floor(10^(n+1)/688) mod 10
    (returns the n‑th digit after the decimal point, 0‑based)
*/

seq_t a(idx_t n)
{
    /* remainder after division by 688, always < 688 */
    seq_t rem = 1;

    for (idx_t i = 0; i <= n; ++i) {
        rem = rem * (seq_t)10;          /* multiply by 10 */
        seq_t digit = rem / (seq_t)688; /* quotient */
        rem   = rem % (seq_t)688;       /* new remainder */
        if (i == n)
            return digit;
    }
    /* unreachable, but keep compiler quiet */
    return 0;
}
