/*
 * A238932  Number of (3+1)X(n+1) 0..3 arrays with no element greater than all
 *          horizontal neighbors or equal to all vertical neighbors.
 * https://oeis.org/A238932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×(n+1) arrays over {0,1,2,3} with no element
 *            greater than all horizontal neighbours and no element equal to
 *            all vertical neighbours
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: DP over columns. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* pre‑computation static data */
    static int M = 0;                       /* number of valid columns (should be 144) */
    static uint8_t *col_val = NULL;         /* flat array of M×4 column values */
    static uint8_t *next_mask = NULL;      /* M×M table: mask of rows where j>i */
    static int *next_tran = NULL;           /* STATES×M table of allowed next column ids */
    static uint8_t *next_cnt = NULL;        /* STATES table of counts (0..M) */
    static seq_t *dp = NULL;                /* DP array for current step  (STATES) */
    static seq_t *dp2 = NULL;               /* DP array for next step    */
    static int STATES = 0;                  /* M × 16                         */

    /* ------------------------------------------------------------------ */
    /* 1. Build the list of all column patterns that satisfy the vertical
       constraint: row0!=row1, row3!=row2,
       and not (row1==row0 && row1==row2),
       not (row2==row1 && row2==row3).                            */
    /* ------------------------------------------------------------------ */
    if (!M) {
        /* temporary buffer for up to 256 patterns */
        int tmp_vals[256][4];
        int total = 0;

        for (int mask = 0; mask < 256; ++mask) {
            int v[4];
            int m = mask;
            for (int r = 0; r < 4; ++r) {
                v[r] = m & 3;
                m >>= 2;
            }
            int ok = 1;
            if (v[0] == v[1]) ok = 0;
            if (v[3] == v[2]) ok = 0;
            if (ok && v[1] == v[0] && v[1] == v[2]) ok = 0;
            if (ok && v[2] == v[1] && v[2] == v[3]) ok = 0;
            if (ok) {
                for (int r = 0; r < 4; ++r) tmp_vals[total][r] = (int) v[r];
                ++total;
            }
        }
        M = total; /* should be 144 */

        /* store columns in a flat byte array */
        col_val = (uint8_t *)malloc(M * 4 * sizeof(uint8_t));
        for (int i = 0; i < M; ++i)
            for (int r = 0; r < 4; ++r)
                col_val[i * 4 + r] = (uint8_t)tmp_vals[i][r];

        /* ------------------------------------------------------------------ */
        /* 2. Pre‑compute next_mask[i][j] = mask of rows where column j >
           column i.  Use a flattened array.                                 */
        /* ------------------------------------------------------------------ */
        next_mask = (uint8_t *)malloc(M * M * sizeof(uint8_t));
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                uint8_t mask = 0;
                for (int r = 0; r < 4; ++r)
                    if (col_val[j * 4 + r] > col_val[i * 4 + r])
                        mask |= (1 << r);
                next_mask[i * M + j] = mask;
            }
        }

        /* ------------------------------------------------------------------ */
        /* 3. Build transition tables for DP states.                           */
        /*    States are encoded as (prev_col << 4) | mask  (mask 0..15).       */
        /* ------------------------------------------------------------------ */
        STATES = M * 16;
        next_cnt = (uint8_t *)malloc(STATES * sizeof(uint8_t));
        next_tran = (int *)malloc(STATES * M * sizeof(int)); /* maximum M entries per state */

        for (int p = 0; p < M; ++p) {
            for (int m = 0; m < 16; ++m) {
                int s = (p << 4) | m;
                int cnt = 0;
                for (int q = 0; q < M; ++q) {
                    int ok = 1;
                    /* rows where current column > previous column must not be
                       followed by a smaller value in the same row.          */
                    for (int r = 0; r < 4; ++r) {
                        if ((m >> r) & 1) {
                            if (col_val[q * 4 + r] < col_val[p * 4 + r]) {
                                ok = 0;
                                break;
                            }
                        }
                    }
                    if (ok) {
                        next_tran[s * M + cnt] = q;
                        ++cnt;
                    }
                }
                next_cnt[s] = (uint8_t)cnt;
            }
        }

        /* ------------------------------------------------------------------ */
        /* 4. Allocate DP arrays.                                               */
        /* ------------------------------------------------------------------ */
        dp  = (seq_t *)malloc(STATES * sizeof(seq_t));
        dp2 = (seq_t *)malloc(STATES * sizeof(seq_t));
        for (int i = 0; i < STATES; ++i) {
            dp[i] = 0;
            dp2[i] = 0;
        }
    }

    /* ------------------------------------------------------------------ */
    /* 5. Main DP for a given n (number of interior columns).              */
    /* ------------------------------------------------------------------ */
    if (n < 1) return 0;              /* sequence defined for n >= 1 */

    idx_t N = n + 1;                  /* total number of columns (including borders) */

    /* 5a. initialise dp with all valid first two columns */
    for (int i = 0; i < STATES; ++i) dp[i] = 0;

    for (int c0 = 0; c0 < M; ++c0) {
        for (int c1 = 0; c1 < M; ++c1) {
            int mask = 0;
            int ok = 1;
            for (int r = 0; r < 4; ++r) {
                uint8_t v0 = col_val[c0 * 4 + r];
                uint8_t v1 = col_val[c1 * 4 + r];
                if (v0 > v1) { ok = 0; break; }  /* left column cannot be greater than right */
                if (v1 > v0) mask |= (1 << r);
            }
            if (!ok) continue;
            int s = (c1 << 4) | mask;
            dp[s] += (seq_t)1;
        }
    }

    /* 5b. add remaining columns 2 .. N-1 */
    for (idx_t col = 2; col < N; ++col) {
        for (int i = 0; i < STATES; ++i) dp2[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t cur = dp[s];
            if (cur == 0) continue;

            int cnt = next_cnt[s];
            int *list = next_tran + s * M;
            for (int t = 0; t < cnt; ++t) {
                int q = list[t];
                /* new mask: rows where q > previous column (p) */
                int p = s >> 4;
                uint8_t nm = next_mask[p * M + q];
                int ns = (q << 4) | nm;
                dp2[ns] += cur;
            }
        }

        /* swap dp and dp2 for next iteration */
        seq_t *tmp = dp;
        dp = dp2;
        dp2 = tmp;
    }

    /* 5c. final answer: last column must not be > left neighbour,
       i.e. mask must be 0. */
    seq_t ans = 0;
    for (int c = 0; c < M; ++c) {
        int s = c << 4;   /* mask == 0 */
        ans += dp[s];
    }

    return ans;
}
