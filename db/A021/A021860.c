/*
 * A021860  Decimal expansion of 1/856.
 * https://oeis.org/A021860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = (floor(10^(n+1) / 856) mod 10)
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth digit after the decimal point in the expansion of 1/856 */

seq_t a(idx_t n)
{
    seq_t rem   = 1;
    seq_t digit = 0;

    /* Perform long division step n+1 times to get the nth digit */
    for (idx_t i = 0; i <= n; ++i) {
        rem   = rem * (seq_t)10;
        digit = rem / (seq_t)856;
        rem   = rem % (seq_t)856;
    }

    return digit;
}
