/*
 * A065707  Bessel polynomial {y_n}'(-2).
 * https://oeis.org/A065707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   * a(n) = y_n'(-2) * where y_n(x) is the nth Bessel polynomial. *
 *            Equivalent explicit sum: * a(n) = ½ · Σ_{j=0}^{n-1} (-1)^j ·
 *            (n+j+1)! /[(n-j-1)!·j!] * Computation uses the recurrence for
 *            the summand: * T_0 = (n+1)·n * T_{j+1} = - T_j · (n+j+2) ·
 *            (n-j-1) / (j+1) (for 0 ≤ j < n-1) * Then a(n) = (Σ...
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset 0 */

    /* T_0 = (n+1)·n  */
    seq_t T   = (seq_t)n * ((seq_t)n + 1);
    seq_t sum = T;

    /* iterate j = 0 … n-2, computing T_{j+1} from T_j */
    for (idx_t j = 0; (j + 1) < n; ++j) {
        /* divide first to keep numbers small */
        seq_t div = T / (seq_t)(j + 1);

        seq_t f1 = (seq_t)n + (seq_t)j + 2;   /* n + j + 2 */
        seq_t f2 = (seq_t)n - (seq_t)j - 1;   /* n - j - 1 */

        seq_t newT = div * f1;
        newT = newT * f2;
        newT = -newT;                         /* multiply by -1 */

        T   = newT;
        sum = sum + T;
    }

    /* final division by 2 (exact) */
    return sum / 2;
}
