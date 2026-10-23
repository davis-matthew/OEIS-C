/*
 * A173936  The determinant of an n X n matrix derived from the matrix X(s,k)
 *          = s^2 - 2*s + k.
 * https://oeis.org/A173936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-2)^(n-2) * (n-1)! * ( H_{n-1} - 2*n*(n-1) ) where
 *            H_{n-1} = Σ_{k=1}^{n-1} 1/k
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;               /* a(1) = 0 by definition */

    /* Compute (n-1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n - 1; ++i) {
        fact *= i;
    }

    /* Compute S1 = (n-1)! * H_{n-1} = Σ (n-1)!/k */
    seq_t S1 = 0;
    for (idx_t k = 1; k <= n - 1; ++k) {
        S1 += fact / (seq_t)k;          /* division is exact for k ≤ n-1 */
    }

    /* Compute S2 = (n-1)! * 2*n*(n-1) */
    seq_t S2 = fact;
    S2 *= 2;
    S2 *= n;
    S2 *= (n - 1);

    /* Difference part */
    seq_t diff = S1 - S2;

    /* Compute (-2)^(n-2) */
    seq_t powNeg2 = 1;
    for (idx_t i = 0; i + 2 < n; ++i) {    /* run n-2 times */
        powNeg2 *= -2;
    }

    return powNeg2 * diff;
}
