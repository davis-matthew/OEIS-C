/*
 * A203350  Number of (n+2)X5 binary arrays avoiding patterns 010 and 101 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A203350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Algorithm: Dynamic programming over rows.
   Counts (n+2)×5 binary matrices avoiding patterns 010 and 101 in rows,
   columns, and NW‑to‑SE diagonals. */

seq_t a(idx_t n) {
    if (n < 1) {
        /* Offset is 1; values for n < 1 are not defined. */
        return (seq_t)0;
    }

    static bool initialized = false;
    static bool row_ok[32];
    static uint32_t trans_tbl[32][32];

    if (!initialized) {
        /* Precompute rows that contain no 010 or 101. */
        for (int mask = 0; mask < 32; ++mask) {
            bool ok = true;
            for (int i = 0; i <= 2; ++i) {
                int pat = (mask >> i) & 7;
                if (pat == 2 || pat == 5) { ok = false; break; }
            }
            row_ok[mask] = ok;
        }

        /* Precompute transitions: given (prev2, prev1) find all admissible
           new rows.  The transition mask indicates which new rows are allowed. */
        for (int p = 0; p < 32; ++p) {
            for (int q = 0; q < 32; ++q) {
                uint32_t mask = 0;
                if (!row_ok[p] || !row_ok[q]) {
                    trans_tbl[p][q] = mask;
                    continue;
                }
                for (int r = 0; r < 32; ++r) {
                    if (!row_ok[r]) continue;

                    bool ok = true;
                    /* Vertical triple constraint. */
                    for (int col = 0; col < 5; ++col) {
                        int b2 = (p >> col) & 1;
                        int b1 = (q >> col) & 1;
                        int b0 = (r >> col) & 1;
                        int pat = (b2 << 2) | (b1 << 1) | b0;
                        if (pat == 2 || pat == 5) { ok = false; break; }
                    }
                    if (!ok) continue;

                    /* NW‑to‑SE diagonal triple constraint (length ≥3). */
                    for (int col = 2; col < 5; ++col) {
                        int b2 = (p >> (col - 2)) & 1;
                        int b1 = (q >> (col - 1)) & 1;
                        int b0 = (r >> col) & 1;
                        int pat = (b2 << 2) | (b1 << 1) | b0;
                        if (pat == 2 || pat == 5) { ok = false; break; }
                    }
                    if (ok) mask |= 1u << r;
                }
                trans_tbl[p][q] = mask;
            }
        }
        initialized = true;
    }

    idx_t h = n + 2;                /* number of rows in the matrix */

    seq_t dp[32][32]   = {{0}};
    seq_t nextdp[32][32] = {{0}};

    /* Initial states: choose any two admissible rows. */
    for (int first = 0; first < 32; ++first) {
        if (!row_ok[first]) continue;
        for (int second = 0; second < 32; ++second) {
            if (!row_ok[second]) continue;
            dp[first][second] = (seq_t)1;
        }
    }

    /* Process rows 3 … h. */
    for (idx_t row = 3; row <= h; ++row) {
        /* Clear nextdp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                nextdp[i][j] = (seq_t)0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int cur = 0; cur < 32; ++cur) {
                seq_t val = dp[prev][cur];
                if (val == 0) continue;
                uint32_t mask = trans_tbl[prev][cur];
                for (int next = 0; next < 32; ++next) {
                    if (mask & (1u << next)) {
                        nextdp[cur][next] += val;
                    }
                }
            }
        }

        /* Swap dp and nextdp for the next iteration. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* Sum all final states. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            total += dp[i][j];

    return total;
}
