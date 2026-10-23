/*
 * A183348  Number of nX6 binary arrays with each 1 adjacent to exactly one 0
 *          vertically and one 0 horizontally.
 * https://oeis.org/A183348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×6 binary arrays in which every 1 has exactly one
 *            adjacent 0 vertically and one adjacent 0 horizontally. Computed
 *            by a transfer‑matrix DP over rows: horizontal validity of each
 *            row is checked locally, and vertical constraints are enforced
 *            when a new row is added. No closed form is used
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define WIDTH   6
#define MASKS   (1 << WIDTH)
#define STATES (MASKS * MASKS)

/* Local static tables – built once */
static bool   hor_valid[MASKS];
static uint8_t trans_next[MASKS][MASKS][MASKS];
static uint8_t trans_cnt[MASKS][MASKS];

/* Helper to initialise tables once */
static void init_tables(void) {
    static bool done = false;
    if (done) return;
    /* Horizontal validity */
    for (int mask = 0; mask < MASKS; ++mask) {
        bool ok = true;
        for (int j = 0; j < WIDTH; ++j) {
            if (!(mask & (1 << j))) continue;       // 0 at j: no constraint
            if (j == 0) {                     /* left edge */
                if (mask & (1 << 1)) { ok = false; break; }
            } else if (j == WIDTH - 1) {      /* right edge */
                if (mask & (1 << (WIDTH - 2))) { ok = false; break; }
            } else {                          /* interior */
                int left  = (mask >> (j - 1)) & 1;
                int right = (mask >> (j + 1)) & 1;
                if (left == right) { ok = false; break; }
            }
        }
        hor_valid[mask] = ok;
    }
    /* Transition table */
    for (int a = 0; a < MASKS; ++a) {
        for (int b = 0; b < MASKS; ++b) {
            trans_cnt[a][b] = 0;
            if (!hor_valid[a] || !hor_valid[b]) continue;
            for (int c = 0; c < MASKS; ++c) {
                if (!hor_valid[c]) continue;
                /* (a ^ c) & b == b  <=>  for every bit of b=1, a and c differ */
                if (((a ^ c) & b) == b) {
                    unsigned char idx = trans_cnt[a][b];
                    trans_next[a][b][idx] = (uint8_t)c;
                    trans_cnt[a][b] = idx + 1;
                }
            }
        }
    }
    done = true;
}

seq_t a(idx_t n) {
    init_tables();

    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)1;   /* only the all‑zero row is allowed */

    seq_t dp_cur[STATES];
    seq_t dp_next[STATES];
    for (int i = 0; i < STATES; ++i) dp_cur[i] = (seq_t)0;

    /* initialise first two rows – top constraint a & b == 0 */
    for (int a = 0; a < MASKS; ++a) {
        if (!hor_valid[a]) continue;
        for (int b = 0; b < MASKS; ++b) {
            if (!hor_valid[b]) continue;
            if ((a & b) != 0) continue;
            dp_cur[a * MASKS + b] += (seq_t)1;
        }
    }

    if (n == 2) {
        seq_t result = (seq_t)0;
        for (int i = 0; i < STATES; ++i) result += dp_cur[i];
        return result;
    }

    /* DP over remaining rows */
    for (int step = 3; step <= n; ++step) {
        for (int i = 0; i < STATES; ++i) dp_next[i] = (seq_t)0;
        for (int a = 0; a < MASKS; ++a) {
            for (int b = 0; b < MASKS; ++b) {
                seq_t val = dp_cur[a * MASKS + b];
                if (!val) continue;
                unsigned char cnt = trans_cnt[a][b];
                for (int k = 0; k < cnt; ++k) {
                    int c = trans_next[a][b][k];
                    dp_next[b * MASKS + c] += val;
                }
            }
        }
        for (int i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
    }

    /* bottom constraint: previous row and last row must have no common 1s */
    seq_t result = (seq_t)0;
    for (int a = 0; a < MASKS; ++a) {
        for (int b = 0; b < MASKS; ++b) {
            if ((a & b) != 0) continue;
            result += dp_cur[a * MASKS + b];
        }
    }
    return result;
}
