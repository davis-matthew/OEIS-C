/*
 * A048800  E.g.f. satisfies A(x) = 1 + x * A(x / (1 - x)).
 * https://oeis.org/A048800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) = n!*A000110(n-1) (n>0), a(0)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute the (n‑1)th Bell number B_{n-1} using Bell’s triangle.
       Two rows of size n are sufficient. */
    idx_t max_size = n;                 /* enough to hold the largest row */
    seq_t *prev = (seq_t*)malloc(max_size * sizeof(seq_t));
    seq_t *cur  = (seq_t*)malloc(max_size * sizeof(seq_t));

    if (!prev || !cur)           /* allocation failure: return 0 */
    {
        if (prev) free(prev);
        if (cur)  free(cur);
        return (seq_t)0;
    }

    size_t prev_sz = 1;
    prev[0] = (seq_t)1;           /* B0 = 1 */

    for (idx_t row = 1; row <= n - 1; ++row)   /* build rows up to B_{n-1} */
    {
        cur[0] = prev[prev_sz - 1];            /* first element of new row */

        for (idx_t col = 1; col <= row; ++col)
            cur[col] = cur[col - 1] + prev[col - 1];

        /* prepare for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
        prev_sz = row + 1;
    }

    seq_t bell = prev[0];      /* B_{n-1} */

    free(cur);
    free(prev);

    /* Compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    return fact * bell;
}
