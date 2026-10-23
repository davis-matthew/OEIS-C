/*
 * A151582  Number of permutations of 1..n arranged in a circle with exactly 9
 *          adjacent element pairs in decreasing order.
 * https://oeis.org/A151582
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A(n-1, 8) where A(N, K) is the Eulerian number
 *            counting permutations of {1..N} with exactly K ascents (or
 *            equivalently K+1 cyclic descents). For the problem we need the
 *            number of permutations with 9 decreasing adjacent pairs in a
 *            circle, which corresponds to 9 cyclic descents i.e. k =...
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 13, u64 n <= 21, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external libraries needed; rely on seq_t and idx_t definitions. */
seq_t a(idx_t n)
{
    /* For n <= 8 there are fewer than 9 elements, so the count is zero. */
    if (n <= 8)
        return (seq_t)0;

    /* N = n-1 is the size for which we need the Eulerian number A(N,8). */
    idx_t N = n - 1;

    /* Arrays holding A(current, k) and A(previous, k) for k = 0..8. */
    seq_t prev[9] = {0};
    seq_t curr[9] = {0};

    /* Initial row for n = 1: only one permutation with 0 ascents. */
    prev[0] = (seq_t)1;
    for (idx_t k = 1; k < 9; ++k)
        prev[k] = (seq_t)0;

    /* Build rows iteratively up to n = N. */
    for (idx_t i = 2; i <= N; ++i) {
        curr[0] = (seq_t)1;                 /* A(i,0) = 1 always. */
        idx_t maxk = (i - 1 < 8) ? (i - 1) : (idx_t)8; /* k cannot exceed i-1 or 8. */

        for (idx_t k = 1; k <= maxk; ++k) {
            curr[k] = (seq_t)(i - k) * prev[k - 1] + (seq_t)(k + 1) * prev[k];
        }
        /* For k > maxk up to 8 set to zero (not needed but keeps array clean). */
        for (idx_t k = maxk + 1; k < 9; ++k)
            curr[k] = (seq_t)0;

        /* Copy current row to prev for next iteration. */
        for (idx_t k = 0; k < 9; ++k)
            prev[k] = curr[k];
    }

    /* After the loop, prev[8] = A(N,8). */
    return (seq_t)n * prev[8];
}
