/*
 * A128808  Coefficients of the invariant of degree 20 associated with the
 *          icosahedral group.
 * https://oeis.org/A128808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) is the coefficient of x^(20-5n) * y^(5n) in the icosahedral
 *            invariant H = -(x^20 + y^20) + 228*(x^15*y^5 - x^5*y^15) -
 *            494*x^10*y^10. There are only five nonzero coefficients: -1,
 *            228, -494, -228, -1 for n=0,1,2,3,4. All other n return 0
 * Range:     i8 n <= 0, i16 n <= 4, i32 n <= 4, i64 n <= 4, i128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n>4 the invariant has no term, so the coefficient is 0. */
    if (n > 4u) return 0;

    /* Coefficients for n = 0..4 in order. */
    static const seq_t coeffs[5] = {
        (seq_t)-1,
        (seq_t)228,
        (seq_t)-494,
        (seq_t)-228,
        (seq_t)-1
    };
    return coeffs[n];
}
