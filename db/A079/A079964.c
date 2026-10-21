/*
 * A079964  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=5, I={0,4}.
 * https://oeis.org/A079964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + a(n-3) + a(n-4) + a(n-6),
   with a(0) = 1, a(1) = 0, a(2) = 1, a(3) = 1, a(4) = 2, a(5) = 2.  This counts
   compositions of n into the parts {2,3,4,6}. */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *dp   = NULL;      /* memoisation array   */
    static size_t  cap  = 0;        /* current allocated size   */
    static size_t  last = 0;        /* highest index computed (inclusive) */

    /* Initialise storage if first call */
    if (dp == NULL) {
        /* start with room for a few terms */
        cap = 64;
        dp  = (seq_t*)malloc(cap * sizeof(seq_t));
        if (dp == NULL) exit(1);
        /* initialise all entries to 0 */
        for (size_t i = 0; i < cap; ++i) dp[i] = 0;
        dp[0] = 1;          /* a(0) */
        dp[1] = 0;          /* a(1) */
        last = 1;           /* we have computed up to index 1 */
    }

    /* Grow array if needed */
    size_t need = (size_t)n;
    if (need >= cap) {
        size_t newcap = cap;
        while (newcap <= need) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(dp, newcap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        /* zero any newly allocated entries */
        for (size_t i = cap; i < newcap; ++i) tmp[i] = 0;
        dp   = tmp;
        cap  = newcap;
    }

    /* Compute new entries up to n on demand */
    while (last < need) {
        ++last;
        seq_t v = 0;
        if (last >= 2) v += dp[last-2];
        if (last >= 3) v += dp[last-3];
        if (last >= 4) v += dp[last-4];
        if (last >= 6) v += dp[last-6];
        dp[last] = v;
    }

    return dp[need];
}
