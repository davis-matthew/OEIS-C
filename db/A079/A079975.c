/*
 * A079975  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=4, I={3}.
 * https://oeis.org/A079975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-5) for n >= 4 * Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=4 * This implements the number of
 *            compositions of n into {1,2,3,5}
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute a(n) using memoisation and the recurrence. */
seq_t a(idx_t n)
{
    /* static storage for computed values */
    static seq_t *dp = NULL;          /* array of a[0..max_idx] */
    static idx_t max_idx = 0;         /* highest index computed so far (inclusive) */
    static idx_t dp_size = 0;         /* allocated size of dp */

    /* initialise for the first call: compute a[0..3] */
    if (dp == NULL) {
        dp_size = 4;
        dp = (seq_t *)malloc(dp_size * sizeof(seq_t));
        /* safety check – abort on allocation failure */
        if (dp == NULL) abort();

        dp[0] = 1;
        dp[1] = 1;
        dp[2] = 2;
        dp[3] = 4;
        max_idx = 3;
    }

    /* grow array if necessary */
    if (n > max_idx) {
        idx_t new_size = dp_size;
        while (new_size <= n)
            new_size <<= 1;          /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        dp = tmp;
        dp_size = new_size;

        /* extend: compute from max_idx+1 up to n */
        for (idx_t i = max_idx + 1; i <= n; ++i) {
            seq_t val = 0;
            if (i >= 1) val += dp[i - 1];
            if (i >= 2) val += dp[i - 2];
            if (i >= 3) val += dp[i - 3];
            if (i >= 5) val += dp[i - 5];  /* dp[-1] and dp[0..] handled naturally */
            dp[i] = val;
        }
        max_idx = n;
    }
    return dp[n];
}
