/*
 * A021882  Decimal expansion of 1/878.
 * https://oeis.org/A021882
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

/* Computation: a(n) is the nth digit after the decimal point of 1/878 */
seq_t a(idx_t n)
{
    /* the fraction is 1/878.  We generate decimal digits by long division:
       start with remainder = 1.  For each position i:
          r *= 10
          digit[i] = r / 878
          r      = r % 878
       This works for any n independently of previous calls. */
    idx_t r = 1;                 /* remainder after the current step (0 <= r < 878) */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;                 /* multiply remainder by 10 for the next digit */
        digit = (seq_t)(r / 878);/* integer division yields the digit (0..9) */
        r = r % 878;             /* keep the new remainder */
    }
    return digit;                /* return the digit as seq_t */
}
