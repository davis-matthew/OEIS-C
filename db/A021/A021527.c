/*
 * A021527  Decimal expansion of 1/523.
 * https://oeis.org/A021527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth digit after the decimal point of 1/523
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1/523 in decimal has a repeating cycle.  We generate digits
       by long division: starting with remainder 1, repeatedly
       multiply by 10, divide by 523, keep the quotient as the
       current digit, and the remainder for the next step.  The
       loop performs n+1 iterations so that the last computed
       digit corresponds to a(n).  All intermediate values stay
       well below 5230, easily fitting into any seq_t that can
       represent the digits 0‑9. */
    seq_t rem = 1;                /* current remainder   */
    seq_t digit = 0;              /* digit for position i */

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;                /* bring down next digit */
        digit = rem / (seq_t)523; /* quotient is the decimal digit */
        rem   = rem % (seq_t)523; /* new remainder for next step */
    }
    return digit;
}
