/*
 * A214580  The hyper-Wiener index of the circumcoronene H(n) (see definition
 *          in the Klavzar papers).
 * https://oeis.org/A214580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/10)n + (17/15)n^2 - 3n^3 - (55/6)n^4 + (82/5)n^5 +
 *            (548/15)n^6
 * Range:     u8 none, u16 n <= 1, u32 n <= 12, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* not used for offset 1, but safe */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;

    /* build numerator in numerator = 3*n + 34*n^2 + 492*n^5 + 1096*n^6
                           - ( 90*n^3 + 275*n^4 )   */
    seq_t pos = 3*nn;
    pos += 34*n2;
    pos += 492*n5;
    pos += 1096*n6;

    seq_t neg = 90*n3;
    neg += 275*n4;

    seq_t num = pos - neg;             /* pos >= neg for all n >= 1 */

    return num / (seq_t)30;            /* divide by LCM of denominators */
}
