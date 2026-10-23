/*
 * A167155  Exponential primorial constant Sum_{k>=0} 1/A140319(k).
 * https://oeis.org/A167155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, easy, nice, nonn
 * Formula:   a(n) = digit n of the decimal expansion of * the rational number
 *            56640643/35156250. * a(1) is the integer part (1), a(2) is the
 *            first digit after the decimal point, etc
 * Range:     u8 none, u16 none, u32 n <= 105, u64 n <= 105, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The exact sum of the first four terms of the series */
    static const seq_t NUM = (seq_t)56640643U;      /* numerator   */
    static const seq_t DEN = (seq_t)35156250U;      /* denominator */

    if (n == 1)
        return NUM / DEN;      /* integer part, which is 1 */

    /* compute the n‑th decimal digit */
    seq_t rem   = NUM % DEN;   /* fractional remainder */
    seq_t digit = 0;

    for (idx_t i = 1; i <= n - 1; ++i) {
        rem   = rem * (seq_t)10;
        digit = rem / DEN;
        rem   = rem % DEN;
    }
    return digit;
}
