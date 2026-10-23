/*
 * A105072  Number of permutations on [n] whose local maxima are in ascending
 *          order.
 * https://oeis.org/A105072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations on [n] whose local maxima are in
 *            ascending order. The following implements the recurrence A[0] =
 *            1, A[m] = 2*A[m-1] + Σ_{j=0}^{m-2} C(m-1,j)*2^{m-j-2}*A[j],
 *            where a(n) = A[n-1] for n ≥ 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1: a(1) is first term */

    idx_t target = n - 1;          /* zero‑based index */

    static seq_t *A      = NULL;   /* memoised values */
    static idx_t   cap   = 0;      /* array capacity */
    static idx_t   size  = 0;      /* number of computed elements */

    /* initialise array if first call */
    if (size == 0) {
        cap  = 1;
        A    = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!A) exit(1);
        A[0] = (seq_t)1;          /* A[0] = 1 */
        size = 1;
    }

    /* grow array to hold up to target */
    if (target + 1 > cap) {
        idx_t new_cap = cap ? cap * 2 : 1;
        while (new_cap < target + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        A   = tmp;
        cap = new_cap;
    }

    /* compute missing terms */
    while (size <= target) {
        idx_t   m   = size;         /* next m to compute */
        seq_t   sum = 0;

        if (m > 1) {
            seq_t power = pow2(m - 2);   /* 2^{m-2} */
            seq_t binom = 1;             /* C(m-1,0) */

            for (idx_t j = 0; j <= m - 2; ++j) {
                seq_t term = binom * power * A[j];
                sum += term;
                if (j < m - 2) {
                    /* update binom and power for next j */
                    binom = binom * (seq_t)(m - 1 - j) / (seq_t)(j + 1);
                    power = power / 2;
                }
            }
        }
        A[m] = A[m - 1] * 2 + sum;
        ++size;
    }

    return A[target];
}
