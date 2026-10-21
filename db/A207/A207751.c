/*
 * A207751  Number of nX7 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = # of n×7 binary matrices avoiding 
 * horizontally 001 and 011, and vertically 001 and 100.
 *
 * Compute using transfer‑matrix DP with states consisting of the last two
 * rows.  There are 20 admissible rows of length 7; the state space is
 * 20×20 = 400.  For n≥3 the recurrence is
 *
 *   a_n = sum_{(p,q)} DP_n[p][q]
 *   DP_2[p][q] = 1                           (all pairs allowed)
 *   DP_{k+1}[q][r] = Σ_{p} DP_k[p][q]  for all r allowed after (p,q)
 *
 * The DP is performed in O(n) time.  All arithmetic is done in seq_t.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper macro to get 1-bit at position i (0 = LSB) of integer x */
#define BIT(x,i) (((x) >> (i)) & 1)

static bool _initialized = false;
static int _rows[20];                      /* list of admissible rows */
static int _R = 0;                         /* number of admissible rows */
static int _trans[400][20];                /* allowed next rows for each pair */
static int _trans_cnt[400];                /* number of allowed transitions */

/* Precompute all admissible rows and transitions */
static void _init(void) {
    if (_initialized) return;
    _initialized = true;

    /* Find all horizontal‑valid rows */
    for (int x = 0; x < 128; ++x) {        /* 7‑bit rows: 0..127 */
        bool ok = true;
        for (int i = 0; i < 5; ++i) {      /* check patterns of length 3 */
            if ((BIT(x,i) == 0) && (BIT(x,i+1) == 0) && (BIT(x,i+2) == 1))
                { ok = false; break; }
            if ((BIT(x,i) == 0) && (BIT(x,i+1) == 1) && (BIT(x,i+2) == 1))
                { ok = false; break; }
        }
        if (ok) _rows[_R++] = x;
    }

    /* For each pair of rows (p,q) precompute allowed next rows (r) */
    for (int p = 0; p < _R; ++p) {
        for (int q = 0; q < _R; ++q) {
            int id = p * _R + q;
            _trans_cnt[id] = 0;
            for (int r = 0; r < _R; ++r) {
                bool ok = true;
                for (int col = 0; col < 7; ++col) {
                    int bp = BIT(_rows[p], col);
                    int bq = BIT(_rows[q], col);
                    int br = BIT(_rows[r], col);
                    /* forbid 0 0 1 and 1 0 0 vertically */
                    if (bp == 0 && bq == 0 && br == 1) { ok = false; break; }
                    if (bp == 1 && bq == 0 && br == 0) { ok = false; break; }
                }
                if (ok) {
                    _trans[id][_trans_cnt[id]++] = r;
                }
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    _init();

    /* n == 1 : only one row */
    if (n == 1) return (seq_t)_R;

    /* n == 2 : any pair of rows */
    if (n == 2) return (seq_t)_R * (seq_t)_R;

    const idx_t pair_cnt = (idx_t)_R * (idx_t)_R;

    /* Allocate DP arrays */
    seq_t *cur = (seq_t *)calloc(pair_cnt, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(pair_cnt, sizeof(seq_t));

    /* Initialize DP for length 2: all pairs count = 1 */
    for (idx_t id = 0; id < pair_cnt; ++id) {
        cur[id] = 1;
    }

    /* Iterate from length 3 up to n */
    for (idx_t len = 3; len <= n; ++len) {
        for (idx_t id = 0; id < pair_cnt; ++id) {
            seq_t val = cur[id];
            if (val == 0) continue;
            int p = (int)(id / _R);
            int q = (int)(id % _R);
            int idx_talk = p * _R + q;
            int cnt = _trans_cnt[idx_talk];
            for (int t = 0; t < cnt; ++t) {
                int r = _trans[idx_talk][t];
                idx_t new_id = (idx_t)q * (idx_t)_R + (idx_t)r;
                nxt[new_id] += val;
            }
        }
        /* swap cur and nxt, and clear nxt */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
        for (idx_t i = 0; i < pair_cnt; ++i) nxt[i] = 0;
    }

    /* Sum all states to get a(n) */
    seq_t result = 0;
    for (idx_t id = 0; id < pair_cnt; ++id) {
        result += cur[id];
    }

    free(cur);
    free(nxt);
    return result;
}
