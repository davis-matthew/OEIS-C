/*
 * A101604  a(n) = 2*a(n-1) + 5*a(n-2) + 2*a(n-3).
 * https://oeis.org/A101604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) + 2*a(n-3) Initial values: a(0) = 1,
 *            a(1) = 2, a(2) = 9
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoization buffer for computed terms */
    static seq_t *memo = NULL;
    static idx_t cap   = 0;      /* allocated capacity */
    static idx_t last  = 0;      /* highest index that has been computed */

    /* Initial allocation and base values */
    if (memo == NULL) {
        cap = 8;                                /* start with some room */
        memo = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!memo) return (seq_t)0;              /* allocation failure */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)2;
        memo[2] = (seq_t)9;
        last   = 2;
    }

    /* Grow buffer if we need to compute larger indices */
    if (n > last) {
        /* Grow capacity if necessary */
        idx_t need = n + 1;
        if (need > cap) {
            idx_t newcap = cap * 2;
            while (newcap < need) newcap *= 2;
            seq_t *tmp = (seq_t *)realloc(memo, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* allocation failure */
            memo = tmp;
            cap  = newcap;
        }

        /* Compute terms from last+1 up to n */
        for (idx_t i = last + 1; i <= n; ++i) {
            memo[i] = (seq_t)2 * memo[i-1]
                    + (seq_t)5 * memo[i-2]
                    + (seq_t)2 * memo[i-3];
        }
        last = n;
    }

    return memo[n];
}
