/*
 * A108368  Coefficients of x/(1-3*x-3*x^2-x^3).
 * https://oeis.org/A108368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) + a(n-3) (for n ≥ 3) Initial values:
 *            a(0)=0, a(1)=1, a(2)=3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *memo = NULL;          /* array of sequence values       */
    static idx_t memo_cap = 0;          /* allocated capacity             */
    static idx_t memo_n   = 0;          /* highest index already computed */

    /* initialise cache with first three terms */
    if (memo == NULL) {
        memo_cap = 3;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        memo[0] = (seq_t)0;
        memo[1] = (seq_t)1;
        memo[2] = (seq_t)3;
        memo_n   = 2;
    }

    /* if the requested index is already cached, return it */
    if (n <= memo_n)
        return memo[n];

    /* grow the cache if necessary */
    if (n >= memo_cap) {
        idx_t new_cap = memo_cap * 2;
        if (new_cap <= n)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (tmp == NULL)   /* allocation failure – return 0 (unlikely) */
            return (seq_t)0;
        memo = tmp;
        memo_cap = new_cap;
    }

    /* compute missing terms up to index n using the recurrence */
    for (idx_t i = memo_n + 1; i <= n; ++i) {
        /* i ≥ 3 because memo_n is at least 2 */
        memo[i] = (seq_t)3 * memo[i-1]
                + (seq_t)3 * memo[i-2]
                + memo[i-3];
    }
    memo_n = n;

    return memo[n];
}
