/*
 * A206985  Number of nX6 0..1 arrays avoiding the patterns 0 1 0 or 1 0 1 in
 *          any row, column, diagonal or antidiagonal.
 * https://oeis.org/A206985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of n×6 0..1 arrays avoiding the patterns
 * 0 1 0 or 1 0 1 in any row, column, diagonal or antidiagonal.
 * Computation is performed by a transfer‑matrix dynamic programming
 * over 6‑bit rows.  The DP state consists of the last two rows of the
 * array; transitions are precomputed for all allowed rows.
 */

#include <stddef.h>
#include <stdbool.h>

/* Store the allowed 6‑bit row masks and transition data.
 * These are built on the first call to a() and reused afterwards.
 */
static size_t Rcnt = 0;                /* number of allowed rows (≤26)       */
static int   rows[26];                 /* masks of allowed rows              */
static bool  trans[676][26];           /* trans[pair][next]  -> true if (a,b,c) valid */

/* Build the list of allowed rows and the transition table.  */
static void build_tables(void)
{
    size_t r = 0;
    /* enumerate all 6‑bit patterns and keep those without 010 or 101 */
    for (int mask = 0; mask < 64; ++mask) {
        bool good = true;
        for (int j = 0; j <= 3; ++j) {
            int t = (mask >> j) & 7;           /* 3‑bit window */
            if (t == 2 || t == 5) {          /* 010 or 101 */
                good = false;
                break;
            }
        }
        if (good) {
            rows[r++] = mask;
        }
    }
    Rcnt = r;

    /* Precompute transitions: for each pair of rows (a,b) and next row c,
     * test all column, diagonal, and antidiagonal 3‑bit triples. */
    for (size_t i = 0; i < Rcnt; ++i) {
        int a = rows[i];
        for (size_t j = 0; j < Rcnt; ++j) {
            int b = rows[j];
            for (size_t k = 0; k < Rcnt; ++k) {
                int c = rows[k];
                bool ok = true;
                /* columns */
                for (int col = 0; col < 6 && ok; ++col) {
                    int t = ((a >> col) & 1) << 2 |
                            ((b >> col) & 1) << 1 |
                            ((c >> col) & 1);
                    if (t == 2 || t == 5) ok = false;
                }
                /* main diagonals: columns j-2, j-1, j for j=2..5 */
                for (int col = 2; col < 6 && ok; ++col) {
                    int t = ((a >> (col - 2)) & 1) << 2 |
                            ((b >> (col - 1)) & 1) << 1 |
                            ((c >> col) & 1);
                    if (t == 2 || t == 5) ok = false;
                }
                /* anti‑diagonals: columns j+2, j+1, j for j=0..3 */
                for (int col = 0; col <= 3 && ok; ++col) {
                    int t = ((a >> (col + 2)) & 1) << 2 |
                            ((b >> (col + 1)) & 1) << 1 |
                            ((c >> col) & 1);
                    if (t == 2 || t == 5) ok = false;
                }
                trans[i * Rcnt + j][k] = ok;
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static bool built = false;
    if (!built) { build_tables(); built = true; }

    /* Handle n=1 and n=2 directly. */
    if (n == 1) return (seq_t)Rcnt;
    if (n == 2) return (seq_t)Rcnt * (seq_t)Rcnt;

    /* Dynamic programming over rows.  dp[state] holds the number of ways
     * to fill the first (row-1) rows ending with the last two rows
     * represented by state = i*Rcnt + j.  For row 2 (two rows filled),
     * every pair of allowed rows is valid, so dp is initialized to 1
     * for all pairs. */
    static seq_t dp[676];
    static seq_t newdp[676];

    /* For clarity, set all dp entries to 0 before use. */
    for (size_t idx = 0; idx < Rcnt * Rcnt; ++idx)
        dp[idx] = (seq_t)0;

    /* Initialize for row 2 (two rows). */
    for (size_t i = 0; i < Rcnt; ++i)
        for (size_t j = 0; j < Rcnt; ++j)
            dp[i * Rcnt + j] = (seq_t)1;

    /* Process rows 3 through n.  At each step, newdp holds counts for
     * the state of the last two rows after adding a new row. */
    for (idx_t row = 3; row <= n; ++row) {
        for (size_t idx = 0; idx < Rcnt * Rcnt; ++idx)
            newdp[idx] = (seq_t)0;

        for (size_t i = 0; i < Rcnt; ++i) {
            for (size_t j = 0; j < Rcnt; ++j) {
                size_t state = i * Rcnt + j;
                seq_t ways = dp[state];
                if (ways == 0) continue;
                for (size_t k = 0; k < Rcnt; ++k) {
                    if (trans[state][k]) {
                        size_t newstate = j * Rcnt + k;
                        newdp[newstate] += ways;
                    }
                }
            }
        }

        /* copy newdp back into dp for the next iteration */
        for (size_t idx = 0; idx < Rcnt * Rcnt; ++idx)
            dp[idx] = newdp[idx];
    }

    /* Sum all dp entries: each represents a full n×6 array. */
    seq_t ans = (seq_t)0;
    for (size_t idx = 0; idx < Rcnt * Rcnt; ++idx)
        ans += dp[idx];

    return ans;
}
