/*
 * A130033  Fourth (m=3) column sequence of triangle A129467.
 * https://oeis.org/A130033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * ((n+2)!)^2 * (2*(n+2) - (n+3)*h(n+2,2))
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = n + 2, the factorial argument */
    idx_t m = n + 2;

    /* Compute (n+2)! as seq_t */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= m; ++i)
        fact = fact * (seq_t)i;

    /* s2 = (n+2)!^2 */
    seq_t s2 = fact * fact;

    /* T = sum_{j=1}^{m} s2 / j^2 */
    seq_t T = (seq_t)0;
    for (idx_t j = 1; j <= m; ++j) {
        seq_t denom = (seq_t)(j * j);          /* j^2, fits in 64‑bit */
        seq_t term  = s2 / denom;             /* exact division */
        T = T + term;
    }

    /* result = 2*m*s2 - (m+1)*T, sign = (-1)^n */
    seq_t result = (seq_t)(2 * m) * s2 - (seq_t)(m + 1) * T;
    if (n & (idx_t)1)          /* odd n => negate */
        result = -result;

    return result;
}
