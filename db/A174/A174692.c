/*
 * A174692  Triangle read by rows: T(n,k) = n! * Eulerian(n + 1, k)^2 - k! +
 *          1.
 * https://oeis.org/A174692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   T(n,k) = n! * Eulerian(n+1, k)^2 - (n! - 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 29, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/factorial.h"

seq_t a(idx_t idx)
{
    /* Find row r and column k such that idx = total_before_row(r) + k */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= idx)
        ++r;
    idx_t k = idx - r * (r + 1) / 2;

    /* Compute n! for n = r */
    seq_t fact = factorial(r);

    /* Compute Eulerian(n+1, k), i.e. Eulerian(r+1, k) */
    seq_t euler;
    if (r == 0) {
        /* Eulerian(1,0) = 1 */
        euler = (seq_t)1;
    } else {
        /* Use two rolling rows of Eulerian numbers */
        size_t size = (size_t)(r + 1); /* max length needed */
        seq_t *prev = (seq_t *)malloc(size * sizeof(seq_t));
        seq_t *curr = (seq_t *)malloc(size * sizeof(seq_t));
        if (!prev || !curr) {
            /* Fallback in pathological OOM case */
            if (prev) free(prev);
            if (curr) free(curr);
            return (seq_t)0;
        }

        /* Base row n = 1: Eulerian(1,0) = 1 */
        prev[0] = (seq_t)1;

        for (size_t n = 2; n <= (size_t)(r + 1); ++n) {
            size_t rowSize = n; /* indices 0 .. n-1 */
            for (size_t j = 0; j < rowSize; ++j) {
                seq_t term1 = 0, term2 = 0;
                if (j > 0) term1 = prev[j - 1] * (seq_t)(n - j);
                if (j < n - 1) term2 = prev[j] * (seq_t)(j + 1);
                curr[j] = term1 + term2;
            }
            /* swap prev and curr */
            seq_t *tmp = prev;
            prev = curr;
            curr = tmp;
        }

        euler = prev[(size_t)k];
        free(prev);
        free(curr);
    }

    /* Compute the triangle value */
    seq_t sq  = euler * euler;
    seq_t prod = sq * fact;
    seq_t sub  = fact - (seq_t)1;
    return prod - sub;
}
