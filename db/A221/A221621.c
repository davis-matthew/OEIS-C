/*
 * A221621  Number of nX6 arrays with each row a permutation of 1..6 having at
 *          least as many downsteps as the preceding row.
 * https://oeis.org/A221621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation for A221621.
   Let F[d] be the Eulerian numbers for permutations of size 6:
   F = {1, 57, 302, 302, 57, 1},  d = 0..5.
   For i ≥ 1 and d = 0..5 let dp[i][d] be the number of
   i×6 arrays ending with a permutation having d descents.
   Then
          dp[1][d] = F[d]
          dp[i][d] = F[d] * Σ_{k=0..d} dp[i-1][k]   (i ≥ 2)
   The required term a(n) = Σ_{d=0..5} dp[n][d].
   This algorithm runs in O(n) time and O(1) additional memory. */
 
seq_t a(idx_t n)
{
    static const seq_t F[6] = {1, 57, 302, 302, 57, 1};
    seq_t cur[6], nxt[6];

    /* row 1 initialization */
    for (idx_t d = 0; d < 6; ++d)
        cur[d] = F[d];

    if (n == 1) {
        seq_t sum = 0;
        for (idx_t d = 0; d < 6; ++d)
            sum += cur[d];
        return sum;
    }

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cum = 0;
        for (idx_t d = 0; d < 6; ++d) {
            cum += cur[d];          /* Σ_{k=0..d} cur[k] */
            nxt[d] = F[d] * cum;    /* DP recurrence */
        }
        /* copy nxt to cur (use explicit assignment for seq_t) */
        for (idx_t d = 0; d < 6; ++d)
            cur[d] = nxt[d];
    }

    seq_t total = 0;
    for (idx_t d = 0; d < 6; ++d)
        total += cur[d];
    return total;
}
