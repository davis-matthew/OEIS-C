/*
 * A088181  Expansion of e.g.f.: 1/(1-sinh(x)-x).
 * https://oeis.org/A088181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; Recurrence: a(n) = n*a(n-1) + Σ_{k=0}^{n-1} binom(n,k)
 *            * a(k) * s(n-k), where s(m) = 1 if m is odd, else 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* includes */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed terms */
    static seq_t *A = NULL;      /* array of a values */
    static idx_t max_n = 0;      /* last computed index */
    static size_t cap = 0;       /* allocated capacity */

    /* on first call: allocate and set a(0)=1 */
    if (A == NULL) {
        cap = 1;
        A = (seq_t *)malloc(cap * sizeof(seq_t));
        if (A == NULL) return (seq_t)0;   /* allocation failed – return 0 */
        A[0] = (seq_t)1;
    }

    /* ensure the cache is large enough */
    if ((idx_t)cap <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;   /* allocation failed – return 0 */
        A = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        if (m == 0) {            /* a(0) is already set */
            A[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;   /* binom(m,0) */

        for (idx_t k = 0; k < m; ++k) {
            if ((m - k) & 1U) {  /* (m-k) is odd → s(m-k)=1 */
                sum += binom * A[k];
            }
            /* update binom to binom(m,k+1) if needed */
            if (k + 1 < m) {
                binom = binom * (seq_t)(m - k) / (seq_t)(k + 1);
            }
        }

        A[m] = (seq_t)m * A[m - 1] + sum;
    }

    max_n = n;
    return A[n];
}
