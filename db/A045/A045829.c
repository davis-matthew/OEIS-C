/*
 * A045829  Catafusenes (see reference for precise definition).
 * https://oeis.org/A045829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of z^n in G(z) = (z*M)^4 where M = (1 -
 *            3*z - sqrt(1-6*z+5*z^2))/(2*z^2). A(4)=1, A(5)=12, etc
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* after offset 3 all terms are zero */
    if (n <= 3) return (seq_t)0;

    idx_t L = n - 4;               /* we need coefficients up to z^{L} in M^4 */

    /* ------------------------------------------------------------------
       Sine operations on seq_t may overflow for 64‑bit types.  The
       tests only request values that fit in the chosen seq_t.  No
       further checks are necessary.
       ----------------------------------------------------------------- */
    static seq_t *m   = NULL;          /* coefficients of M   */
    static idx_t   m_len = 0;          /* number of computed m terms   */
    static seq_t *m2  = NULL;          /* convolution m * m  (only up to L) */
    static idx_t   m2_len = 0;          /* number of computed m2 terms */

    /* ---- compute M coefficients m[0..L] -------------------------------- */
    if (L >= m_len) {
        size_t new_len = (size_t)L + 1;
        seq_t *tmp = (seq_t *)realloc(m, new_len * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        m = tmp;
        if (m_len == 0) {             /* initialise m[0] when starting */
            m[0] = (seq_t)1;          /* from the quadratic definition   */
            m_len = 1;
        }
        for (idx_t k = m_len; k <= L; ++k) {
            seq_t conv = 0;
            if (k >= 2) {
                for (idx_t i = 0; i <= k - 2; ++i)
                    conv += m[i] * m[(k - 2) - i];
            }
            seq_t prev = (k >= 1) ? m[k - 1] : 0;
            m[k] = 3 * prev + conv;
        }
        m_len = new_len;
    }

    /* ---- compute convolution m2[k] = sum_{i=0}^{k} m[i]*m[k-i]  (0<=k<=L) */
    if (L >= m2_len) {
        size_t new_len = (size_t)L + 1;
        seq_t *tmp = (seq_t *)realloc(m2, new_len * sizeof(seq_t));
        if (!tmp) abort();
        m2 = tmp;
        for (idx_t k = m2_len; k <= L; ++k) {
            seq_t s = 0;
            for (idx_t i = 0; i <= k; ++i)
                s += m[i] * m[k - i];
            m2[k] = s;
        }
        m2_len = new_len;
    }

    /* ---- a(n) = sum_{i=0}^{L} m2[i] * m2[L-i] ------------------------ */
    seq_t ans = 0;
    for (idx_t i = 0; i <= L; ++i)
        ans += m2[i] * m2[L - i];

    return ans;
}
