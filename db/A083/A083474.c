/*
 * A083474  a(n) = T(n)^2-n!, where T(n) is the n-th triangular number.
 * https://oeis.org/A083474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = (n*(n+1)/2)^2 - n!
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute triangular number T(n) = n*(n+1)/2 */
    seq_t triangular = (seq_t)n * ((seq_t)n + 1) / 2;

    /* square the triangular number */
    seq_t tri_sq = triangular * triangular;

    /* compute factorial n! iteratively */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= i;
    }

    /* a(n) = T(n)^2 - n! */
    return tri_sq - fact;
}
