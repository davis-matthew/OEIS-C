/*
 * A060996  Stirling2 transform of [2,3,3,3,3,3,3,3,...].
 * https://oeis.org/A060996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) = 3*Bell(n+1) - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Derive Bell(n+1) using Aitken's (Bell) triangle */
    /* Bell(0) = 1 is the first entry of the triangle. */
    /* Row i (0‑based) gives Bell(i+1) as its last element. */

    /* Convert index to size_t for array handling. */
    size_t N = (size_t)n;          /* we need to compute up to row N (i.e., Bell(N+1)) */
    /* Allocate two rows of size N+1 each. */
    seq_t *prev = (seq_t*)malloc(sizeof(seq_t) * (N + 1));
    seq_t *cur  = (seq_t*)malloc(sizeof(seq_t) * (N + 1));
    if (!prev || !cur)
    {
        /* In an actual implementation we would handle allocation failure.
           Here we simply return 0. */
        return (seq_t)0;
    }

    /* Initial state: row 0 contains just Bell(0)=1. */
    prev[0] = (seq_t)1;

    size_t i;
    for (i = 1; i <= N; ++i)
    {
        /* First element of new row is last element of previous row. */
        cur[0] = prev[i - 1];

        size_t j;
        for (j = 1; j <= i; ++j)
        {
            /* cur[j] = cur[j-1] + prev[j-1];  (only additions) */
            cur[j] = cur[j - 1] + prev[j - 1];
        }

        /* Swap pointers: cur becomes the new prev for next iteration. */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* After the loop, prev[N] holds Bell(N+1). */
    seq_t bell = prev[N];

    /* Clean up temporary array (which holds the old prev). */
    free(cur);

    /* a(n) = 3*Bell(n+1) - 1 */
    seq_t result = bell * (seq_t)3;
    result = result - (seq_t)1;

    return result;
}
