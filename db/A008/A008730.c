/*
 * A008730  Molien series 1/((1-x)^2*(1-x^12)) for 3-dimensional group [2,n] =
 *          *22n.
 * https://oeis.org/A008730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = Sum_{j=0..n+12} floor(j/12)
 * Range:     u8 n <= 71, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = n + 12 */
    seq_t m = (seq_t)n + (seq_t)12;
    /* t = floor(m/12), r = m mod 12 */
    seq_t t = m / (seq_t)12;
    seq_t r = m % (seq_t)12;
    /* Compute a(n) = t * (6*t + r - 5) */
    seq_t bracket = (seq_t)6 * t + r - (seq_t)5;
    return t * bracket;
}
