/*
 * A129464  Second column (m=1) of triangle A129462 (v=2 member of a certain
 *          family).
 * https://oeis.org/A129464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; for n>0: a(n) = -(n-1)!^2 * n * (n+1)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 12, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i < n; ++i) {    /* compute (n-1)! */
        fact *= (seq_t)i;
    }

    seq_t result = -fact * fact;          /* -(n-1)!^2 */
    result *= (seq_t)n;                   /* * n */
    result *= (seq_t)(n + 1);             /* * (n+1) */
    return result;
}
