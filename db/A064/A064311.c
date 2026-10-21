/*
 * A064311  Generalized Catalan numbers C(-2; n).
 * https://oeis.org/A064311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/n) * Σ_{m=0}^{n-1} (n-m)*binomial(n-1+m, m)*(-2)^m,
 *            a(0) = 1
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 11, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;         /* Σ of terms (n-m)*P_m        */
    seq_t P   = 1;         /* P_m = binomial(n-1+m,m)*(-2)^m, m=0 */

    for (idx_t m = 0; m < n; ++m) {
        /* term = (n-m) * P_m */
        seq_t term = P * (seq_t)(n - m);
        sum += term;

        /* update P for the next m:
           P_{m+1} = P_m * (n + m) * (-2) / (m + 1)            */
        if (m + 1 < n) {
            P = (P * (seq_t)(n + m)) * (seq_t)(-2);
            P /= (seq_t)(m + 1);
        }
    }

    /* divide the accumulated sum by n (exact division) */
    return sum / (seq_t)n;
}
