/*
 * A053504  Number of degree-n permutations of order dividing 24.
 * https://oeis.org/A053504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑k∈{1,2,3,4,6,8,12,24} ((n−1)!/(n−k)!)·a(n−k), a(0)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
# include <new>
#endif

seq_t a(idx_t n)
{
    /* Allowed cycle lengths */
    const idx_t lengths[8] = {1,2,3,4,6,8,12,24};

    /* Static memoization table */
    static seq_t *dp = NULL;
    static idx_t dp_cap = 0;        /* allocated capacity */
    static idx_t dp_computed = 0;   /* largest n computed so far */

    /* First time: allocate table */
    if (dp == NULL) {
        idx_t init_cap = (n + 1 > 32) ? n + 1 : 32;
        if (init_cap < 1) init_cap = 1;
#ifdef __cplusplus
        dp = new seq_t[init_cap];
#else
        dp = (seq_t *)calloc(init_cap, sizeof(seq_t));
#endif
        dp_cap = init_cap;
        dp_computed = 0;
        dp[0] = (seq_t)1;  /* a(0)=1 */
    }

    /* Grow table if necessary */
    if ((idx_t)(n + 1) > dp_cap) {
        idx_t new_cap = dp_cap;
        while ((idx_t)(n + 1) > new_cap) new_cap <<= 1;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (idx_t i = 0; i < dp_cap; ++i) tmp[i] = dp[i];
        delete[] dp;
        dp = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        dp = tmp;
#endif
        dp_cap = new_cap;
    }

    /* Compute required values */
    for (idx_t i = dp_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (int li = 0; li < 8; ++li) {
            idx_t k = lengths[li];
            if (k > i) continue;
            seq_t prod = 1;
            /* prod = (i-1)!/(i-k)! = product of j = i-k+1 .. i-1 */
            for (idx_t j = i - k + 1; j <= i - 1; ++j)
                prod *= (seq_t)j;
            sum += dp[i - k] * prod;
        }
        dp[i] = sum;
    }
    if (dp_computed < n) dp_computed = n;

    return dp[n];
}
