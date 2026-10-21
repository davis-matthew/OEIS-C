/*
 * A207247  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by a transfer‑matrix DP over column patterns (vertical
 *            and horizontal avoidance rules)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 37, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define V            31                 /* number of valid 7‑bit column patterns   */
#define PAIRS        (V * V)            /* ordered pairs of columns                */
#define MAX_BITMASK  128                /* 2^7                               */

/* Tables built once on first call. */
static uint8_t  valid_cols[V];              /* list of valid column bitmasks          */
static uint8_t  col_idx[MAX_BITMASK];      /* map bitmask -> index (0..V-1)           */
static uint8_t  allowed_next[PAIRS][V];      /* indices of next column per pair        */
static uint8_t  allowed_cnt[PAIRS];          /* count of allowed next columns          */

/* DP tables (grown only once). */
static seq_t dp_cur[PAIRS];
static seq_t dp_next[PAIRS];

static void build_tables(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* ---- 1. Enumerate all 7‑bit columns that satisfy the vertical rule. */
    int vcnt = 0;
    for (int mask = 0; mask < MAX_BITMASK; ++mask) {
        bool ok = true;
        for (int r = 0; r <= 4 && ok; ++r) {   /* 3‑row window */
            int pat = (mask >> r) & 7;        /* bits r..r+2    */
            if (pat == 1 || pat == 2)         /* 001 or 010 forbidden */
                ok = false;
        }
        if (ok) {
            valid_cols[vcnt] = (uint8_t)mask;
            col_idx[mask] = (uint8_t)vcnt;
            vcnt++;
        }
    }
    /* In a correct build vcnt == V. */

    /* ---- 2. Precompute allowed next columns for each ordered pair. */
    for (int i = 0; i < PAIRS; ++i) {
        uint8_t c1_idx = i / V;
        uint8_t c2_idx = i % V;
        uint8_t c1 = valid_cols[c1_idx];
        uint8_t c2 = valid_cols[c2_idx];
        uint8_t cnt = 0;
        for (int j = 0; j < V; ++j) {
            uint8_t c3 = valid_cols[j];
            bool ok = true;
            for (int r = 0; r < 7 && ok; ++r) {
                int pat = ((c1 >> r) & 1) << 2 |
                          ((c2 >> r) & 1) << 1 |
                          ((c3 >> r) & 1);
                if (pat == 0 || pat == 3)   /* 000 or 011 forbidden */
                    ok = false;
            }
            if (ok) {
                allowed_next[i][cnt++] = (uint8_t)j; /* store column index j */
            }
        }
        allowed_cnt[i] = cnt;
    }
}

/* -------------------------------------------------------------------- */
/* Sequence function.                                                   */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* below offset, not required but harmless */

    build_tables();               /* ensure tables built once */

    /* n = 1: any valid column */
    if (n == 1) return (seq_t)V;

    /* Length 2: each pair contributes one way. */
    for (int i = 0; i < PAIRS; ++i) dp_cur[i] = (seq_t)1;

    /* Build up to length n. */
    for (idx_t col = 3; col <= n; ++col) {
        /* zero dp_next */
        for (int i = 0; i < PAIRS; ++i) dp_next[i] = 0;

        /* Transition from each existing pair. */
        for (int pair = 0; pair < PAIRS; ++pair) {
            seq_t ways = dp_cur[pair];
            if (ways == 0) continue;
            uint8_t upto = allowed_cnt[pair];
            const uint8_t *list = allowed_next[pair];
            uint8_t c2_idx = pair % V;
            for (uint8_t t = 0; t < upto; ++t) {
                uint8_t c3_idx = list[t];
                int nxt = c2_idx * V + c3_idx;
                dp_next[nxt] += ways;
            }
        }
        /* swap dp_cur and dp_next */
        for (int i = 0; i < PAIRS; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum all states for final length. */
    seq_t total = 0;
    for (int i = 0; i < PAIRS; ++i) total += dp_cur[i];
    return total;
}
