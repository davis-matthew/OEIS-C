/*
 * A180057  The LCM of the n-th row of the triangle of Eulerian numbers,
 *          A008292.
 * https://oeis.org/A180057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm{A(n,k) : 0 ≤ k < n} (Eulerian numbers)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A(0,0) = 1.  For n>0:  A(n,0)=A(n,n-1)=1 and
 *   A(n,k) = (n-k)*A(n-1,k-1) + (k+1)*A(n-1,k)  (1 ≤ k < n-1). */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) = 1, a(1) = 1, a(2) = 1, … */
    if (n <= 1U)
        return 1U;

    /* Allocate two rows of length n+1 (indices 0..n). */
    seq_t *prev = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return 0;            /* out‑of‑memory; caller should handle */
    }

    /* Initialise row 0: only A(0,0)=1. */
    prev[0] = 1U;
    for (idx_t i = 1; i <= n; ++i)
        prev[i] = 0U;

    /* Build rows 1 … n */
    for (idx_t row = 1; row <= n; ++row) {
        /* first and last elements are 1 */
        curr[0]          = 1U;
        curr[row - 1]    = 1U;

        /* inner elements (if any) */
        for (idx_t k = 1; k < row - 1; ++k) {
            seq_t term1 = ((seq_t)(row - k)) * prev[k - 1];
            seq_t term2 = ((seq_t)(k + 1)) * prev[k];
            curr[k] = term1 + term2;
        }

        /* compute lcm of this row only */
        seq_t lcm_row = 1U;
        for (idx_t k = 0; k < row; ++k) {
            seq_t val = curr[k];
            seq_t g   = gcd(lcm_row, val);
            lcm_row = (lcm_row / g) * val;
        }

        if (row == n) {
            free(prev);
            free(curr);
            return lcm_row;
        }

        /* swap rows for next iteration */
        seq_t *tmp  = prev;
        prev        = curr;
        curr        = tmp;
    }

    /* Should never reach here – n==0 handled above. */
    free(prev);
    free(curr);
    return 1U;
}
