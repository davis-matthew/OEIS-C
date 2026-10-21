/*
 * A223902  Poly-Cauchy numbers of the second kind hat c_n^(-4).
 * https://oeis.org/A223902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * (k+1)^4 * Stirling1(n,k)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 18, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived formula used: a(n) = (-1)^n * Σ_{m=1..5} c_m * |Stirling1(n+1,m)| */
/* with coefficients c = [0,1,15,50,60,24]   (c_m = (m-1)! * Stirling2(5,m)) */

#include <stdint.h>

/* absolute value for seq_t */
static inline seq_t abs_seq_t(seq_t x) {
    return (x < 0) ? -x : x;
}

seq_t a(idx_t n) {
    /* coefficients for m = 1..5 */
    static const seq_t coeff[6] = {0, 1, 15, 50, 60, 24};

    seq_t cur[6] = {1, 0, 0, 0, 0, 0};   /* Stirling1 for current n (start with n = 0) */

    idx_t i;
    for (i = 0; i <= n; ++i) {
        /* compute Stirling1 for n+1 */
        seq_t next[6];
        next[0] = 0;
        for (idx_t m = 1; m <= 5; ++m) {
            next[m] = cur[m - 1] - (seq_t)i * cur[m];
        }

        /* compute a(i) using values for n+1 */
        seq_t sum = 0;
        for (idx_t m = 1; m <= 5; ++m) {
            sum += coeff[m] * abs_seq_t(next[m]);
        }
        seq_t result = (i & 1) ? -sum : sum;

        if (i == n)
            return result;

        /* advance to next n */
        for (idx_t m = 0; m <= 5; ++m)
            cur[m] = next[m];
    }
    return 0; /* unreachable */
}
