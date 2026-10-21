/*
 * A225474  Triangle read by rows, k!*2^k*s_2(n, k) where s_m(n, k) are the
 *          Stirling-Frobenius cycle numbers of order m; n >= 0, k >= 0.
 * https://oeis.org/A225474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle read by rows, where
 *            T(n,k)=k!*2^k*s_2(n,k) and the Stirling‑Frobenius cycle numbers
 *            s_2(n,k) satisfy s_2(0,0)=1
 *            s_2(n,k)=s_2(n-1,k-1)+(2*n-1)*s_2(n-1,k)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* special case n==0 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate arrays for s_2 values (two rows for DP) */
    seq_t *s_prev = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *s_cur  = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!s_prev || !s_cur) {
        free(s_prev); free(s_cur);
        return (seq_t)0;
    }

    /* Zero initialize */
    for (idx_t i = 0; i <= n; ++i) {
        s_prev[i] = 0;
        s_cur[i]  = 0;
    }
    s_prev[0] = 1;                     /* s_2(0,0)=1 */

    /* factorial and pow2 tables up to n */
    seq_t *fact = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *pow2 = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!fact || !pow2) {
        free(s_prev); free(s_cur); free(fact); free(pow2);
        return (seq_t)0;
    }

    fact[0] = 1;
    pow2[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
        pow2[i] = pow2[i-1] * (seq_t)2;
    }

    idx_t idx = 0;                /* current index in the triangle */
    /* first element of row 0 */
    if (idx == n) {
        free(s_prev); free(s_cur); free(fact); free(pow2);
        return (seq_t)1;
    }
    ++idx;

    /* iterate over rows 1..n */
    for (idx_t r = 1; r <= n; ++r) {
        /* compute s_2 for this row */
        s_cur[0] = (seq_t)(2*r - 1) * s_prev[0];
        for (idx_t k = 1; k <= r; ++k) {
            seq_t a_val = s_prev[k-1];
            seq_t b_val = (k <= r-1) ? s_prev[k] : 0;
            s_cur[k] = a_val + (seq_t)(2*r - 1) * b_val;
        }

        /* produce terms of this row */
        for (idx_t k = 0; k <= r; ++k) {
            seq_t term = s_cur[k] * fact[k] * pow2[k];
            if (idx == n) {
                free(s_prev); free(s_cur); free(fact); free(pow2);
                return term;
            }
            ++idx;
        }

        /* swap row buffers for next iteration */
        seq_t *tmp = s_prev;
        s_prev = s_cur;
        s_cur  = tmp;
    }

    /* Should never reach here for valid n */
    free(s_prev); free(s_cur); free(fact); free(pow2);
    return (seq_t)0;
}
