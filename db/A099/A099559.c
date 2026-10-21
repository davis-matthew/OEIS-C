/*
 * A099559  a(n) = Sum_{k=0..floor(n/5)} C(n-4k,k+1).
 * https://oeis.org/A099559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/5⌋} C(n-4k, k+1)
 * Range:     u8 n <= 18, u16 n <= 37, u32 n <= 77, u64 n <= 156,
 *            u128 n <= 313, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the stated recurrence:
   a(n) = 2*a(n-1) – a(n-2) + a(n-5) – a(n-6)
   with initial values a(0)=0, a(1)=1, a(2)=2, a(3)=3,
   a(4)=4, a(5)=5. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *dp = NULL;          /* array of computed values */
    static idx_t  cap = 0;           /* allocated capacity */
    static idx_t  last = 0;          /* last index that has been filled */

    /* Allocate or extend capacity up to n+1 */
    if (dp == NULL) {
        cap = n + 1;
        dp = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!dp) return 0; /* out‑of‑memory: return 0 silently */
    } else if (n + 1 > cap) {
        size_t new_cap = n + 1;
        dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!dp) return 0;
        /* zero‑initialize newly added elements */
        memset(dp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 5) {
            /* initial segment: a(i) = i for i = 0..5 */
            dp[i] = (seq_t)i;
        } else {
            /* recurrence: a(i) = 2*a(i-1) - a(i-2) + a(i-5) - a(i-6) */
            seq_t val = dp[i-1];
            val += dp[i-1];         /* 2*a(i-1) */
            val -= dp[i-2];         /* -a(i-2) */
            val += dp[i-5];         /* +a(i-5) */
            val -= dp[i-6];         /* -a(i-6) */
            dp[i] = val;
        }
    }
    last = n;
    return dp[n];
}
