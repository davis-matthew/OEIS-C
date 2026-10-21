/*
 * A207568  Number of 7Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×n binary matrices avoiding * 0 0 1 and 0 1 0
 *            horizontally, and 0 1 1 and 1 1 0 vertically. * Computed by
 *            dynamic programming on pairs of columns (transfer matrix)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static data kept between calls */
static int allowed_cols[35];
static int allowed_count = 0;
static int trans_count[35][35];
static int trans_next[35][35][35];
static int initialized = 0;

seq_t a(idx_t n) {
    /* One‑time initialization */
    if (!initialized) {
        /* allowed columns: vertical patterns only */
        int idx = 0;
        for (int col = 0; col < (1 << 7); col++) {
            int ok = 1;
            for (int i = 0; i <= 4; i++) {
                int b0 = (col >> i) & 1;
                int b1 = (col >> (i + 1)) & 1;
                int b2 = (col >> (i + 2)) & 1;
                if ((b0 == 0 && b1 == 1 && b2 == 1) ||
                    (b0 == 1 && b1 == 1 && b2 == 0)) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                allowed_cols[idx++] = col;
            }
        }
        allowed_count = idx;          /* should be 35 */

        /* precompute transitions between column pairs */
        for (int i = 0; i < allowed_count; i++) {
            for (int j = 0; j < allowed_count; j++) {
                int cnt = 0;
                int col1 = allowed_cols[i];
                int col2 = allowed_cols[j];
                for (int k = 0; k < allowed_count; k++) {
                    int col3 = allowed_cols[k];
                    int ok = 1;
                    for (int r = 0; r < 7; r++) {
                        int a = (col1 >> r) & 1;
                        int b = (col2 >> r) & 1;
                        int c = (col3 >> r) & 1;
                        if (a == 0 && ((b == 0 && c == 1) || (b == 1 && c == 0))) {
                            ok = 0;
                            break;
                        }
                    }
                    if (ok) {
                        trans_next[i][j][cnt++] = k;
                    }
                }
                trans_count[i][j] = cnt;
            }
        }

        initialized = 1;
    }

    /* base cases */
    if (n == 0) return 0;
    if (n == 1) return (seq_t)allowed_count;
    if (n == 2) return (seq_t)allowed_count * (seq_t)allowed_count;

    /* DP over column pairs */
    const size_t STATE_COUNT = (size_t)allowed_count * allowed_count;   /* 1225 */

    seq_t *cur = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
    if (!cur || !nxt) return 0;   /* allocation failure */

    /* initial state for two columns: every pair allowed once */
    for (size_t s = 0; s < STATE_COUNT; s++) cur[s] = 1;

    for (idx_t step = 3; step <= n; step++) {
        for (size_t s = 0; s < STATE_COUNT; s++) nxt[s] = 0;

        for (size_t s = 0; s < STATE_COUNT; s++) {
            seq_t val = cur[s];
            if (val == 0) continue;
            size_t i = s / allowed_count;
            size_t j = s % allowed_count;
            int cnt = trans_count[i][j];
            for (int t = 0; t < cnt; t++) {
                size_t k = (size_t)trans_next[i][j][t];
                size_t idx_st = j * allowed_count + k;
                nxt[idx_st] = nxt[idx_st] + val;
            }
        }
        /* swap buffers */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum over final states */
    seq_t total = 0;
    for (size_t s = 0; s < STATE_COUNT; s++) {
        total = total + cur[s];
    }

    free(cur);
    free(nxt);
    return total;
}
