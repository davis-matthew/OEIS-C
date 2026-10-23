/*
 * A183066  G.f.: A(x) = (1 + 21*x + 3*x^2 - x^3)/(1-x)^5.
 * https://oeis.org/A183066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 6*n^3 + 11*n^2 + 7*n + 1
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 254, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;

    seq_t term = (seq_t)1;          /* constant term */
    term += (seq_t)7 * x;           /* linear term */
    term += (seq_t)11 * x * x;      /* quadratic term */
    term += (seq_t)6 * x * x * x;   /* cubic term */
    term += x * x * x * x;          /* quartic term */

    return term;
}
