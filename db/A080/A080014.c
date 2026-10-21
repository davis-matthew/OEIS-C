/*
 * A080014  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=2, I={1}.
 * https://oeis.org/A080014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)-a(n-5)-a(n-6), a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=3, a(4)=6, a(5)=10
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 73, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static buffer for computed values */
    static seq_t *memo = NULL;      /* memoized values */
    static idx_t maxComputed = (idx_t)-1;  /* last index that has been computed */
    static const seq_t init[6] = {1, 1, 1, 3, 6, 10};

    /* First call: initialise the buffer with the first 6 terms */
    if (maxComputed == (idx_t)-1) {
        maxComputed = 5;
        memo = (seq_t *)malloc((size_t)(maxComputed + 1) * sizeof(seq_t));
        if (!memo) return 0;     /* out of memory, return 0 as safe fallback */
        memcpy(memo, init, sizeof(init));
    }

    /* If the requested index is already known, return it */
    if (n <= maxComputed) {
        return memo[n];
    }

    /* Expand the memo table to at least n */
    size_t newSize = (size_t)(n + 1);
    seq_t *newMemo = (seq_t *)malloc(newSize * sizeof(seq_t));
    if (!newMemo) return 0;  /* out of memory */
    memcpy(newMemo, memo, (size_t)(maxComputed + 1) * sizeof(seq_t));

    /* Compute new entries from (maxComputed+1) up to n */
    for (idx_t i = maxComputed + 1; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1)+a(i-2)+a(i-3)+a(i-4)-a(i-5)-a(i-6) */
        seq_t sum = newMemo[i - 1] + newMemo[i - 2] + newMemo[i - 3] + newMemo[i - 4];
        seq_t sub = newMemo[i - 5] + newMemo[i - 6];
        newMemo[i] = sum - sub;
    }

    /* Replace old memo array */
    free(memo);
    memo = newMemo;
    maxComputed = n;

    return memo[n];
}
