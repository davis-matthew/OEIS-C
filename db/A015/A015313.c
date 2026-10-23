/*
 * A015313  Gaussian binomial coefficient [ n,5 ] for q = -8.
 * https://oeis.org/A015313
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  sign, easy
 * Formula:   Gaussian binomial coefficient [n,5] for q = -8
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 7, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: [n,k] = [n-1,k] + (-8)^(n-k) * [n-1,k-1]  (k ≥ 1) */
/*  a(n) is [n,5] with a(5)=1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 5) return 0;            /* undefined for n < 5, return 0 */

    /* Allocate array of powers of q = -8 up to exponent n */
    seq_t *pow_q = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!pow_q) return 0;           /* allocation failed */

    pow_q[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow_q[i] = pow_q[i - 1] * (seq_t)-8;   /* q = -8 */

    /* DP array for current values of [i,k], k = 0..5 */
    seq_t c[6] = {0};
    c[0] = 1;                      /* [i,0] = 1 for all i */

    for (idx_t i = 1; i <= n; ++i) {
        /* Update in reverse order to use old c[k-1] values */
        for (int k = 5; k >= 1; --k) {
            if ((idx_t)k > i) continue;   /* skip when k > i */
            c[k] = c[k] + pow_q[i - k] * c[k - 1];
        }
    }

    free(pow_q);
    return c[5];                       /* a(n) = [n,5] */
}
