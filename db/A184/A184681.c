/*
 * A184681  Number of (n+1) X 5 binary arrays with every 2 X 2 subblock
 *          singular.
 * https://oeis.org/A184681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) for OEIS A184681 via DP over 5‑bit rows.
 * Each state is a 5‑bit binary row (0–31).  
 * A transition from row r to row s is allowed iff for every j=0..3  
 * the 2×2 subblock [[r_j, r_{j+1}],[s_j, s_{j+1}]] has determinant zero
 * over GF(2), i.e. r_j·s_{j+1} == r_{j+1}·s_j.  
 * The number of length‑(n+1) arrays is the number of paths of length n
 * in this directed graph.  This DP runs in O(32²·n). */
seq_t a(idx_t n)
{
    /* dp[i] = number of ways to reach row i after current number of steps */
    seq_t dp[32], nxt[32];
    for (int i = 0; i < 32; ++i) dp[i] = 1;          /* any initial row */

    for (idx_t step = 0; step < n; ++step) {
        for (int j = 0; j < 32; ++j) nxt[j] = 0;

        for (int i = 0; i < 32; ++i) {
            if (dp[i] == 0) continue;
            seq_t val = dp[i];
            for (int j = 0; j < 32; ++j) {
                int ok = 1;
                for (int col = 0; col < 4; ++col) {
                    int a1 = (i >> col) & 1;        /* top-left   */
                    int b1 = (i >> (col + 1)) & 1;  /* top-right  */
                    int a2 = (j >> col) & 1;        /* bottom-left*/
                    int b2 = (j >> (col + 1)) & 1;  /* bottom-right*/
                    if ((a1 & b2) != (b1 & a2)) { ok = 0; break; }
                }
                if (ok) nxt[j] += val;
            }
        }
        for (int k = 0; k < 32; ++k) dp[k] = nxt[k];
    }

    seq_t total = 0;
    for (int i = 0; i < 32; ++i) total += dp[i];
    return total;
}
