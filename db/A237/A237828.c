/*
 * A237828  Number of partitions of n such that 2*(least part) + 1 = greatest
 *          part.
 * https://oeis.org/A237828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 139, u32 n <= 495, u64 n <= 1786,
 *            u128 n <= 6652, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A237828
   Formula: a(n) = coefficient of x^n in
             Sum_{k>=1} x^(3k+1) / Prod_{j=k}^{2k+1} (1 - x^j)
   We compute it by maintaining the product H_k(x) = Prod_{j=k}^{2k+1} (1-x^j)^{-1}.
   H_{k+1} = (1 - x^k) * H_k / ((1 - x^{2k+2})(1 - x^{2k+3})).
   At each step we add the contribution of k to the answer array.
   The implementation keeps the result for all indices up to the largest
   value requested so far, re‑computing only when a larger n is asked. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static state: arrays for partitions (H) and answer (A) */
    static seq_t *H = NULL;
    static seq_t *A = NULL;
    static size_t   cap = 0;    /* current capacity (number of indices) */
    static idx_t    maxComputed = 0;   /* largest n computed so far */

    /* if we already have the answer, simply return it */
    if (n <= maxComputed) {
        return A[n];
    }

    /* need to recompute for a larger n: allocate or grow arrays */
    if (n + 1 > cap) {
        /* free old arrays if any */
        free(H);
        free(A);
        cap = (size_t)(n + 1);
        H = (seq_t*)calloc(cap, sizeof(seq_t));
        A = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!H || !A) {   /* allocation failed */
            return (seq_t)0;
        }
    } else {
        /* else just clear A up to n */
        for (idx_t i = 0; i <= n; ++i) A[i] = (seq_t)0;
    }

    /* --- initialise H for k = 1 (parts 1,2,3) --- */
    H[0] = (seq_t)1;
    for (idx_t part = 1; part <= 3; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            H[sum] += H[sum - part];
        }
    }

    /* --- compute contributions for each k --- */
    idx_t cur_k   = 1;
    idx_t shift   = 3 * cur_k + 1;   /* shift for k = 1 is 4 */

    while (shift <= n) {
        /* add contribution of current k */
        for (idx_t i = shift; i <= n; ++i) {
            A[i] += H[i - shift];
        }

        /* prepare H for next k (k+1) */
        ++cur_k;
        idx_t prev = cur_k - 1;   /* part to remove */

        /* multiply by (1 - x^prev): subtract H[i - prev] */
        for (idx_t i = n; i >= prev; --i) {
            H[i] -= H[i - prev];
        }

        /* add new part r1 = 2*prev + 2 */
        idx_t r1 = 2 * prev + 2;
        if (r1 <= n) {
            for (idx_t i = r1; i <= n; ++i) {
                H[i] += H[i - r1];
            }
        }

        /* add new part r2 = 2*prev + 3 */
        idx_t r2 = r1 + 1;   /* since r2 = 2*prev + 3 */
        if (r2 <= n) {
            for (idx_t i = r2; i <= n; ++i) {
                H[i] += H[i - r2];
            }
        }

        /* update shift for new k */
        shift = 3 * cur_k + 1;
    }

    /* record max computed n */
    maxComputed = n;
    return A[n];
}
