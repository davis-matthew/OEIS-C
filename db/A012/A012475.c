/*
 * A012475  a(n) = (2*n+1) [x^(2*n+1)] arctan(cos(x)*sin(x)).
 * https://oeis.org/A012475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   A(0,k) = 1; * A(n,k) = -(k*(k+1)*A(n-1,k+2) + 4*k^2*A(n-1,k))
 *            (n>0) * a(n) = A(n,1) (n>=0) * This implements the recurrence
 *            directly using dynamic programming. * The function works for any
 *            seq_t (int8_t…__int128 or an arbitrary‑ * precision type) and
 *            any idx_t (usually uint64_t). It is * cache‑fr...
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                 /* a(0) = 1 */

    /* Allocate two auxiliary tables of size n+1 (worst case). */
    seq_t *prev = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!prev || !curr)          /* defensive: never expected to fail in tests */
        return 0;                /* return zero as a graceful fallback */

    /* Base row: A(0,k) = 1 for all k (indexed by odd numbers). */
    for (idx_t i = 0; i <= n; ++i)
        prev[i] = 1;

    /* Iterate over the rows of the recurrence table. */
    for (idx_t m = 1; m <= n; ++m) {
        idx_t limit = n - m;     /* number of entries to compute in this row
                                   (indices 0 … limit). */

        for (idx_t idx = 0; idx <= limit; ++idx) {
            /* k is the odd integer corresponding to this index. */
            seq_t k = (seq_t)(2 * idx + 1);

            /* term = k*(k+1)*A(n-1,k+2)  +  4*k^2*A(n-1,k)  */
            seq_t term1 = k * (k + 1) * prev[idx + 1];
            seq_t term2 = ((seq_t)4 * k * k) * prev[idx];
            curr[idx] = -(term1 + term2);
        }

        /* Prepare for the next iteration: swap the buffers. */
        seq_t *tmp  = prev;
        prev        = curr;
        curr        = tmp;
    }

    /* After completing n steps, prev[0] holds A(n,1) = a(n). */
    seq_t result = prev[0];

    free(prev);
    free(curr);
    return result;
}
