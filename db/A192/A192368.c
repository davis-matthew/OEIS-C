/*
 * A192368  Number of lattice paths from (0,0) to (n,n) using steps (1,0),
 *          (2,0), (0,2), (1,1).
 * https://oeis.org/A192368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of lattice paths from (0,0) to (n,n) using steps (1,0),
 *            (2,0), (0,2), (1,1). Returns a(n) for index n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    size_t sz = (size_t)(n + 1);

    seq_t *prev2 = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *prev1 = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *cur   = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!prev2 || !prev1 || !cur) {
        /* allocation failure – return zero (should not occur in tests) */
        return (seq_t)0;
    }

    /* initialise row 0: a[0][0] = 1, a[0][j] = 1 if j even else 0 */
    prev1[0] = (seq_t)1;
    for (size_t j = 1; j <= n; ++j)
        prev1[j] = ((j & 1) == 0) ? (seq_t)1 : (seq_t)0;

    seq_t result = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i) {
        for (size_t j = 0; j <= n; ++j) {
            seq_t v = prev1[j];                     /* step (1,0) */
            if (i >= 2) v += prev2[j];              /* step (2,0) */
            if (j >= 2) v += cur[j - 2];            /* step (0,2) */
            if (j >= 1) v += prev1[j - 1];          /* step (1,1) */
            cur[j] = v;
        }
        if (i == n) result = cur[n];
        /* rotate rows for next iteration:
           prev2 <- prev1, prev1 <- cur, cur <- old prev2 */
        seq_t *tmp = prev2;
        prev2 = prev1;
        prev1 = cur;
        cur   = tmp;
    }

    free(prev2);
    free(prev1);
    free(cur);
    return result;
}
