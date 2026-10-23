/*
 * A137731  Repeated set splitting, labeled elements.
 * https://oeis.org/A137731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1}^{n-1} S2(n-1,k) * a(k) * a(n-k) (a(1)=1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization area */
    static idx_t computed = 0;     /* maximum n already computed */
    static seq_t *A = NULL;       /* a[0..computed] ; a[0] unused */
    static size_t A_cap = 0;       /* allocated capacity for A */

    static seq_t *S = NULL;       /* current Stirling row S(n-1,k) for k=0..n-1 */
    static size_t S_cap = 0;       /* allocated capacity for S */

    if (n == 0)          /* out of OEIS range, not used */
        return (seq_t)0;

    /* ensure A has room up to n */
    if (n >= A_cap) {
        size_t new_cap = n + 1;
        if (new_cap < 2) new_cap = 2;
        if (new_cap < A_cap * 2) new_cap = A_cap * 2;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure, unlikely */
        A = tmp;
        /* initialise new entries to 0 */
        for (size_t i = A_cap; i < new_cap; ++i) A[i] = (seq_t)0;
        A_cap = new_cap;
    }

    /* ensure S has room up to n-1 (row for current computed+1) */
    if (n - 1 >= S_cap) {
        size_t new_cap = (n - 1) + 1;
        if (new_cap < 1) new_cap = 1;
        if (new_cap < S_cap * 2) new_cap = S_cap * 2;
        seq_t *tmp = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        S = tmp;
        for (size_t i = S_cap; i < new_cap; ++i) S[i] = (seq_t)0;
        S_cap = new_cap;
    }

    /* Compute all a up to n */
    for (idx_t m = computed + 1; m <= n; ++m) {
        if (m == (idx_t)1) {
            /* initial value */
            A[1] = (seq_t)1;
            /* initialise first Stirling row S(0,0)=0, S(1,1)=1 */
            if (S_cap < 2) {  /* allocate for indices 0 and 1 */
                seq_t *tmp = (seq_t *)realloc(S, 2 * sizeof(seq_t));
                if (!tmp) return (seq_t)0;
                S = tmp;
                S_cap = 2;
            }
            S[0] = (seq_t)0;
            S[1] = (seq_t)1;
            computed = 1;
            continue;
        }

        /* At this point, S holds the Stirling numbers S(m-1,k) for k=1..m-1 */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < m; ++k) {
            /* term = S(m-1,k) * a(k) * a(m-k) */
            seq_t term = S[k];
            term = term * A[k];
            term = term * A[m - k];
            sum   = sum + term;
        }
        A[m] = sum;

        /* Update Stirling row to S(m,k) for next iteration */
        /* Ensure array has space for index m */
        if (m >= S_cap) {
            size_t new_cap = m + 1;
            if (new_cap < 2) new_cap = 2;
            if (new_cap < S_cap * 2) new_cap = S_cap * 2;
            seq_t *tmp = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            S = tmp;
            for (size_t i = S_cap; i < new_cap; ++i) S[i] = (seq_t)0;
            S_cap = new_cap;
        }

        /* Produce new Stirling row in place using the old values of S */
        /* Recurrence: S(m,k) = S(m-1,k-1) + k * S(m-1,k)   for k=1..m-1 */
        seq_t prev = (seq_t)0;           /* old S(m-1,0) */
        for (idx_t k = 1; k < m; ++k) {
            seq_t cur = S[k];          /* old S(m-1,k) */
            S[k] = prev + (seq_t)k * cur;
            prev = cur;                /* set for next iteration */
        }
        S[m] = (seq_t)1;               /* S(m,m) = 1 */
        S[0] = (seq_t)0;               /* not used, keep zero */

        computed = m;
    }

    return A[n];
}
