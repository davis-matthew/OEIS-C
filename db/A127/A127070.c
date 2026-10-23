/*
 * A127070  Define an array by d(m, 0) = 1, d(m, 1) = m; d(m, k) = (m - k + 1)
 *          d(m+1, k-1) - (k-1) (m+1) d(m+2, k-2). Sequence gives d(4,n).
 * https://oeis.org/A127070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = d(4,n) where d(m,0)=1, d(m,1)=m, and d(m,k) =
 *            (m-k+1)*d(m+1,k-1) - (k-1)*(m+1)*d(m+2,k-2)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special cases n = 0, 1 */
    if (n == 0) return 1;
    if (n == 1) return (seq_t)4;      /* d(4,1) = 4 */

    const seq_t r = (seq_t)n + 4;     /* r = 4 + n  (constant along recurrence) */

    /* dp0 = d(r-2, 2) predecessor distances:
       dp0 holds d(m, k-2) for current k, dp1 holds d(m, k-1) */
    seq_t dp0 = 1;                    /* d(m,0) = 1 */
    seq_t dp1 = r - 1;                /* d(m,1) = m with m = r-1 */

    for (idx_t k = 2; k <= n; ++k) {
        /* m = r - k  (current m for this k) */
        const seq_t m_minus = r - 2 * (seq_t)k + 1;    /* (m - k + 1) */
        const seq_t m_plus  = r - (seq_t)k + 1;        /* (m + 1)   */
        const seq_t dk = m_minus * dp1
                       - ((seq_t)k - 1) * m_plus * dp0;

        dp0 = dp1;
        dp1 = dk;
    }
    return dp1;   /* dp1 now holds d(4,n) = a(n) */
}
