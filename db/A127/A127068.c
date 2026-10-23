/*
 * A127068  Let d(m, 0) = 1, d(m, 1) = m, and d(m, k) = (m - k + 1)*d(m+1,
 *          k-1) - (k-1)*(m+1) d(m+2, k-2). Sequence gives d(3,n).
 * https://oeis.org/A127068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 10, i64 n <= 18, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A127068
   Recurrence for odd indices:
      a(1) = 3, a(3) = -30
      a(2k+1) = -2*(2k+3)*(3k-2)*a(2k-1) 
                -4*(k-1)*(2k+3)*(4k^2-1)*a(2k-3)
   Closed form for even indices (k = n/2):
      a(2k) = (-1)^(k+1)*(k+1)*(2k-1)*(2k)!
*/
#include <stdbool.h>   // required for bool

/* Helper: factorial for even indices */
static seq_t fact_upto(idx_t m) {
    seq_t f = 1;
    for (idx_t i = 2; i <= m; ++i) {
        f *= (seq_t)i;
    }
    return f;
}

seq_t a(idx_t n) {
    /* Special‑case the first two terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    if ((n & 1) == 0) {                /* even n : a(2k) */
        idx_t k = n >> 1;                     /* k = n/2 */
        seq_t sign = (((k + 1) & 1) ? (seq_t)-1 : (seq_t)1);
        seq_t prod = (seq_t)(k + 1) * (seq_t)(2 * k - 1);
        seq_t fact = fact_upto(2 * k);             /* (2k)! */
        return sign * prod * fact;
    } else {                           /* odd n : a(2k+1) */
        /* initialize with a(1)=3, a(3)=-30 */
        seq_t a_prev2 = (seq_t)3;
        seq_t a_prev4 = (seq_t)-30;
        /* m = (n-1)/2 gives the position in the odd recurrence */
        idx_t m = (n - 1) >> 1;                /* m >= 1 */
        if (m == 1) return a_prev4;            /* a(3) already known */

        for (idx_t cur = 2; cur <= m; ++cur) { /* cur runs from 2 to m */
            /* term1 = -2*(2*cur+3)*(3*cur-2)*a(2*cur-1) */
            seq_t term1 = -(seq_t)2 * (seq_t)(2 * cur + 3)
                           * (seq_t)(3 * cur - 2) * a_prev4;
            /* term2 = -4*(cur-1)*(2*cur+3)*(4*cur^2-1)*a(2*cur-3) */
            seq_t factor2 = (seq_t)4 * (seq_t)(cur - 1)
                            * (seq_t)(2 * cur + 3)
                            * (seq_t)(4 * cur * cur - 1);
            seq_t term2 = -factor2 * a_prev2;
            seq_t curr = term1 + term2;
            a_prev2 = a_prev4;
            a_prev4 = curr;
        }
        return a_prev4;
    }
}
