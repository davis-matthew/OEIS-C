/*
 * A185610  Number of (n+1)X2 0..2 arrays with every 2X2 subblock sum equal to
 *          some horizontal or vertical neighbor 2X2 subblock sum.
 * https://oeis.org/A185610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n≥2 a(n) counts the (n+1)×2 arrays over {0,1,2} for
 *            which every 2×2 subblock sum equals at least one of its vertical
 *            neighboring subblock sums. This is equivalent to counting
 *            sequences of row‑sums R₀…Rₙ (each in {0,…,4}) satisfying
 *            R_{i‑1}=R_{i+1} or R_i=R_{i+2} (1≤i≤n‑1), together wi...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* a(1) = 0 (no valid 2×2 array) */
    if (n < 2) return 0;

    /* number of patterns for each row sum */
    const seq_t f[5] = {1, 2, 3, 2, 1};

    seq_t total = 0;            /* final answer */

    /* iterate over all possibilities for the two initial row sums */
    for (idx_t s0 = 0; s0 < 5; ++s0) {
        for (idx_t s1 = 0; s1 < 5; ++s1) {

            /* product of pattern counts for the first three rows
               (R₂ is forced to equal R₀ by the boundary constraint) */
            seq_t init = f[s0] * f[s1] * f[s0];

            /* DP tables: dp[a][b][c] holds the current number of ways
               with the last three row sums equal to a,b,c respectively */
            seq_t dp[5][5][5]   = {{{0}}};
            seq_t dp_next[5][5][5] = {{{0}}};

            dp[s0][s1][s0] = init;

            /* positions 3 .. n correspond to rows R₃ … Rₙ (indexing from 0) */
            for (idx_t pos = 3; pos <= n; ++pos) {
                /* zero dp_next */
                for (idx_t a = 0; a < 5; ++a)
                    for (idx_t b = 0; b < 5; ++b)
                        for (idx_t c = 0; c < 5; ++c)
                            dp_next[a][b][c] = 0;

                /* transition: try all possible next row sums d */
                for (idx_t a = 0; a < 5; ++a)
                    for (idx_t b = 0; b < 5; ++b)
                        for (idx_t c = 0; c < 5; ++c) {
                            seq_t cnt = dp[a][b][c];
                            if (!cnt) continue;
                            for (idx_t d = 0; d < 5; ++d) {
                                if ( (a == c) || (b == d) ) {      /* validity test */
                                    dp_next[b][c][d] += cnt * f[d];
                                }
                            }
                        }

                /* swap dp and dp_next */
                for (idx_t a = 0; a < 5; ++a)
                    for (idx_t b = 0; b < 5; ++b)
                        for (idx_t c = 0; c < 5; ++c)
                            dp[a][b][c] = dp_next[a][b][c];
            }

            /* After filling all rows, enforce the last boundary constraint
               Rₙ = R_{n‑2}, which is equivalent to a == c in the final state. */
            for (idx_t a = 0; a < 5; ++a)
                for (idx_t b = 0; b < 5; ++b)
                    for (idx_t c = 0; c < 5; ++c)
                        if (a == c)
                            total += dp[a][b][c];
        }
    }

    return total;
}
