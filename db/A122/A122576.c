/*
 * A122576  Expansion of g.f.: -x*(1 - 2*x + 6*x^2 - 2*x^3 +
 *          x^4)/((1-x)^3*(1+x)^4).
 * https://oeis.org/A122576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(n+1)/8 * ((2*n+1)*(-1)^n - 1)
 * Range:     i8 n <= 7, i16 n <= 50, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t term = nn * (nn + 1);
    seq_t sgn  = ((n & 1) ? (seq_t)-1 : (seq_t)1);
    seq_t inner = (( (seq_t)2 * nn + (seq_t)1) * sgn - (seq_t)1);
    return term * inner / (seq_t)8;
}
