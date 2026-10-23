/*
 * A122951  Number of walks from (0,0) to (n,n) in the region x >= y with the
 *          steps (1,0), (0,1), (2,0) and (0,2).
 * https://oeis.org/A122951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, walk
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Code computes a(n), the number of walks from (0,0) to (n,n) that stay in
   the region x >= y using steps (1,0), (0,1), (2,0), (0,2).
   Recurrence (for 0 <= k <= m):
        f(m,k) = (m>=1 && m-1>=k          ? f(m-1,k) : 0)
               + (m>=2 && m-2>=k          ? f(m-2,k) : 0)
               + (k>=1                      ? f(m ,k-1) : 0)
               + (k>=2                      ? f(m ,k-2) : 0);
   Base: f(0,0)=1, f(m,k)=0 for m<k.   The answer a(n)=f(n,n).
   This DP uses three sliding rows, requiring O(n) memory and O(n²) time.
*/

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;                                  /* a(0)=1 */

    size_t len = (size_t)n + 1;
    seq_t *prev2 = (seq_t *)calloc(len, sizeof(seq_t));   /* row m-2   */
    seq_t *prev1 = (seq_t *)calloc(len, sizeof(seq_t));   /* row m-1   */
    seq_t *cur   = (seq_t *)calloc(len, sizeof(seq_t));   /* current   */

    if (!prev2 || !prev1 || !cur) {                      /* allocation failure */
        free(prev2); free(prev1); free(cur);
        return (seq_t)0;
    }

    /* f(0,0)=1 */
    prev1[0] = (seq_t)1;

    for (idx_t m = 1; m <= n; ++m) {
        /* compute cur[0..m]  */
        for (idx_t k = 0; k <= m; ++k) {
            seq_t val = 0;
            if (m >= 1 && (m - 1) >= k)
                val += prev1[k];
            if (m >= 2 && (m - 2) >= k)
                val += prev2[k];
            if (k >= 1)
                val += cur[k - 1];
            if (k >= 2)
                val += cur[k - 2];
            cur[k] = val;
        }

        if (m == n) {          /* answer ready */
            seq_t result = cur[m];
            free(prev2); free(prev1); free(cur);
            return result;
        }

        /* rotate rows for next iteration */
        seq_t *tmp  = prev2;
        prev2       = prev1;
        prev1       = cur;
        cur         = tmp;     /* reuse previous prev2 buffer */
    }

    /* unreachable, but keeps compiler happy */
    free(prev2); free(prev1); free(cur);
    return (seq_t)0;
}
