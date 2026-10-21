/*
 * A229228  Number of set partitions of {1,...,2n} into sets of size at most
 *          n.
 * https://oeis.org/A229228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)! * [x^(2n)] exp(Sum_{j=1..n} x^j/j!)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of set partitions of {1,…,2n} into blocks of size at most n. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Memoisation of previously computed values */
    static seq_t *memo = NULL;
    static bool *computed = NULL;
    static size_t memo_cap = 0;

    size_t needed = (size_t)n + 1;
    if (needed > memo_cap) {
        size_t new_cap = memo_cap ? memo_cap * 2 : 1;
        while (new_cap < needed) new_cap *= 2;

        seq_t *new_mem = (seq_t *)malloc(new_cap * sizeof(seq_t));
        bool *new_comp = (bool *)malloc(new_cap * sizeof(bool));
        if (memo) {
            memcpy(new_mem, memo, memo_cap * sizeof(seq_t));
            memcpy(new_comp, computed, memo_cap * sizeof(bool));
            free(memo);
            free(computed);
        }
        memset(new_comp + memo_cap, 0, (new_cap - memo_cap) * sizeof(bool));
        memo      = new_mem;
        computed  = new_comp;
        memo_cap  = new_cap;
    }

    if (computed[n]) return memo[n];

    /* Compute S_n(2n) via recurrence:
       S_k(m) = sum_{t=1..min(k,m)} C(m-1, t-1) * S_k(m-t)
    */
    size_t mmax = (size_t)n * 2;
    seq_t *dp = (seq_t *)malloc((mmax + 1) * sizeof(seq_t));
    dp[0] = (seq_t)1;  /* S_k(0) = 1 */

    for (size_t m = 1; m <= mmax; ++m) {
        size_t limit = m < (size_t)n ? m : (size_t)n;
        seq_t sum = 0;
        seq_t comb = 1;  /* C(m-1,0) */

        for (size_t t = 1; t <= limit; ++t) {
            /* sum += dp[m-t] * comb */
            sum += dp[m - t] * comb;

            /* Update comb = C(m-1, t) for next t */
            if (t < limit) {
                /* comb = comb * (m - t) / t */
                comb = (comb * (seq_t)(m - t)) / (seq_t)t;
            }
        }
        dp[m] = sum;
    }

    seq_t result = dp[mmax];
    free(dp);

    memo[n] = result;
    computed[n] = true;
    return result;
}
