/*
 * A221618  Number of n X n arrays with each row a permutation of 1..n having
 *          at least as many downsteps as the preceding row.
 * https://oeis.org/A221618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over nondecreasing sequences (d1..dn) of * ∏_{i=1..n}
 *            Eulerian(n, di) * * Computation: 1) compute all Eulerian numbers
 *            for size n (A(n,k)) * 2) DP over rows to accumulate counts of
 *            arrays with nondecreasing * descent numbers. Complexity O(n²),
 *            memory O(n)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n == 0) return (seq_t)0;       /* offset starts at 1, but guard */
    if (n == 1) return (seq_t)1;

    /* ---------- 1. Compute Eulerian numbers A(n,k) for k=0..n-1 ---------- */
    /* Two temporary rows for the recurrence */
    seq_t *prev = (seq_t *)malloc(n * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc(n * sizeof(seq_t));
    if (!prev || !cur) return (seq_t)0;  /* allocation failure */

    /* base case A(1,0)=1 */
    prev[0] = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i) {
        /* boundaries */
        cur[0]     = (seq_t)1;
        cur[i-1]   = (seq_t)1;

        /* interior values */
        for (idx_t k = 1; k <= i-2; ++k) {
            seq_t term1 = prev[k-1] * ((seq_t)(i - k));
            seq_t term2 = prev[k]   * ((seq_t)(k + 1));
            cur[k] = term1 + term2;
        }

        /* swap previous and current for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }
    /* after loop, 'prev' holds A(n,k) for k=0..n-1 */

    /* ---------- 2. DP over array rows ---------- */
    seq_t *dp_prev = (seq_t *)malloc(n * sizeof(seq_t));
    seq_t *dp_cur  = (seq_t *)malloc(n * sizeof(seq_t));
    if (!dp_prev || !dp_cur) { free(prev); free(cur); return (seq_t)0; }

    /* initialize dp for first row (row 1) */
    for (idx_t k = 0; k < n; ++k) dp_prev[k] = prev[k];

    /* iterate remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        seq_t cumulative = 0;
        for (idx_t k = 0; k < n; ++k) {
            cumulative += dp_prev[k];
            dp_cur[k] = cumulative * prev[k];
        }
        /* swap dp arrays for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;
    }

    /* ---------- 3. Sum over final descent counts ---------- */
    seq_t result = 0;
    for (idx_t k = 0; k < n; ++k) result += dp_prev[k];

    /* ---------- 4. Clean up ---------- */
    free(prev);
    free(cur);
    free(dp_prev);
    free(dp_cur);

    return result;
}
