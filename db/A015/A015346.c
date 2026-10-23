/*
 * A015346  Gaussian binomial coefficient [ n,7 ] for q = -7.
 * https://oeis.org/A015346
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,7] evaluated at q = -7
 *            (the recurrence [n,k] = [n‑1,k] + q^(n‑k)·[n‑1,k‑1] with q = -7,
 *            k = 7)
 * Range:     i8 n <= 7, i16 n <= 7, i32 n <= 8, i64 n <= 10, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Local arrays for the DP and the powers of q */
    seq_t dp[8] = {0};      /* dp[k] = [current_n, k] */
    seq_t qpow[8] = {0};    /* qpow[k] = q^(current_n - k) */

    dp[0] = (seq_t)1;       /* [n,0] = 1 for all n */
    const seq_t q = (seq_t)-7; /* the value of q */

    /* Iterate from 1 up to n (the result is dp[7] when done) */
    for (idx_t cur = 1; cur <= n; ++cur) {
        int maxk = cur < (idx_t)7 ? (int)cur : 7;   /* k cannot exceed 7 */
        int prevk = (cur - 1) < (idx_t)7 ? (int)(cur - 1) : 7;

        /* Update qpow for the new current_n:
           qpow[k] becomes q^(cur - k) = old_qpow[k] * q          */
        for (int k = prevk; k >= 1; --k) {
            qpow[k] = qpow[k] * q;
        }
        /* For the new column k = cur (if inside the 7‑wide band) set to 1
           because q^(cur-curr) = q^0 = 1                           */
        if (cur <= (idx_t)7) {
            qpow[(int)cur] = (seq_t)1;
        }

        /* Update dp in descending order to avoid overwriting needed values */
        for (int k = maxk; k >= 1; --k) {
            dp[k] = dp[k] + qpow[k] * dp[k - 1];
        }
    }

    /* For indices below 7 the value of a(n) is 0, which already holds in dp[7] */
    return dp[7];
}
