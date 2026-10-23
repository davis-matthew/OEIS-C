/*
 * A021461  Decimal expansion of 1/457.
 * https://oeis.org/A021461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th decimal digit of 1/457 (0-indexed), * obtained
 *            by long division: repeatedly multiply the remainder * by 10 and
 *            take the quotient as a digit
 * Range:     u8 none, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

extern seq_t a(idx_t n) {
    /* The period of the decimal expansion is at most 456. */
    static seq_t digits[457];
    static idx_t period_len = 0;

    if (period_len == 0) {
        const seq_t DEN = 457;
        seq_t r = 1;        /* remainder starts as 1 (numerator of 1/457) */
        idx_t i = 0;
        while (1) {
            r *= 10;                           /* shift decimal place */
            seq_t d = r / DEN;                 /* next digit */
            r %= DEN;                          /* new remainder  */
            digits[i] = d;                     /* store digit */
            i++;
            if (r == 1) {                      /* cycle completes */
                period_len = i;
                break;
            }
        }
    }
    return digits[n % period_len];
}
