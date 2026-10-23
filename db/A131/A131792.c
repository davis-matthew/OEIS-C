/*
 * A131792  Main diagonal of triangle A131791: a(n) = A131791(n,n) for n>=0.
 * https://oeis.org/A131792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{j=0}^{n-1} (1 - x^(2^j+1))/(1-x)
 *            Equivalent: number of ways to write n = Σ_{j=0}^{n-1} t_j with 0
 *            ≤ t_j ≤ 2^j
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------- */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* a(0) = 1 */

    size_t N = (size_t)n;            /* array up to N */
    /* allocate arrays */
    seq_t *prev = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *pref = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!prev || !cur || !pref) {
        /* Allocation failed – return 0 as a safe fallback (unlikely). */
        free(prev); free(cur); free(pref);
        return 0;
    }

    /* initialize prev[0] = 1, others 0 */
    prev[0] = 1;
    for (size_t i = 1; i <= N; ++i)
        prev[i] = 0;

    for (idx_t j = 0; j < n; ++j) {
        /* compute prefix sums of prev into pref */
        pref[0] = prev[0];
        for (size_t s = 1; s <= N; ++s)
            pref[s] = pref[s - 1] + prev[s];

        /* bound = 2^j, but stop when it exceeds N to avoid overflow */
        idx_t bound;
        if (j >= 63)          /* shift would overflow 64‑bit */
            bound = N + 1;    /* effectively “≥ N” */
        else
            bound = (idx_t)1 << j;

        if (bound >= N) {
            /* All coefficients of the range are included; copy pref */
            for (size_t s = 0; s <= N; ++s)
                cur[s] = pref[s];
        } else {
            idx_t span = bound + 1;   /* number of terms in each factor */
            for (size_t s = 0; s <= N; ++s) {
                if (s < span)
                    cur[s] = pref[s];
                else
                    cur[s] = pref[s] - pref[s - span];
            }
        }

        /* swap prev and cur for next iteration */
        seq_t *t = prev;
        prev = cur;
        cur = t;
    }

    seq_t result = prev[n];

    free(prev);
    free(cur);
    free(pref);

    return result;
}
