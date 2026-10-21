/*
 * A071818  Triangle of T(n,k) where T(n,k)/(n-1)! is probability of player k
 *          out of n players winning a game of "Elimination": rules are that
 *          player 1 chooses one of the others at random to be eliminated,
 *          then player 2 (or 3 if player 2 has been eliminated) chooses
 *          somebody else at random from the survivors...
 * https://oeis.org/A071818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) element of the triangular array T(n,k) * read
 *            row by row. The array satisfies * T(1,1) = 1, * T(n,1) = (n-1) *
 *            T(n-1,n-1) (n > 1), * T(n,0) = 0, * T(n,k) = (k-2) * T(n-1,k-2)
 *            + (n-k) * T(n-1,k-1) (1 < k ≤ n). * The function computes a(n)
 *            for all n ≥ 1
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* Treat n < 1 as undefined; return 0 to avoid UB */
    if (n < 1) return 0;

    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* Column index k within that row (1‑based) */
    idx_t prev_sum = r * (r - 1) / 2;   /* sum of lengths of rows before r */
    idx_t k = n - prev_sum;              /* 1 ≤ k ≤ r */

    /* Allocate two rows of length r+1 (index 0 is unused) */
    seq_t *prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        /* Allocation failure: fall back to zero */
        free(prev);
        free(curr);
        return 0;
    }

    /* Initialize first row T(1,1)=1 */
    for (idx_t i = 0; i <= r; ++i) prev[i] = 0;
    prev[1] = 1;

    if (r == 1) {
        seq_t result = prev[1];
        free(curr);
        free(prev);
        return result;
    }

    /* Build rows 2 … r iteratively */
    for (idx_t i = 2; i <= r; ++i) {
        curr[0] = 0;                       /* unused slot */
        curr[1] = (seq_t)(i - 1) * prev[i - 1];

        for (idx_t kk = 2; kk <= i; ++kk) {
            seq_t term1 = ((seq_t)(kk - 2)) * prev[kk - 2];
            seq_t term2 = ((seq_t)(i - kk)) * prev[kk - 1];
            curr[kk] = term1 + term2;
        }

        /* Swap pointers: prev becomes current row for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* After the loop, prev holds the desired row */
    seq_t result = prev[k];

    free(curr);
    free(prev);
    return result;
}
