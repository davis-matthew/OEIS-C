/*
 * A025803  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^7)).
 * https://oeis.org/A025803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + n*(13+7*(-1)^n) + 90 + 50*(-1)^n)/112)
 * Range:     u8 n <= 5, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num;
    if ((n & 1) == 0)   /* n even */
        num = nn * nn + (seq_t)20 * nn + (seq_t)140;
    else                /* n odd */
        num = nn * nn + (seq_t)6  * nn + (seq_t)40;
    return num / (seq_t)112;
}
