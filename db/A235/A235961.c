/*
 * A235961  Number of (n+1)X(7+1) 0..1 arrays with the difference of the upper
 *          and lower median value of each 2X2 subblock in lexicographically
 *          nondecreasing order columnwise and nonincreasing rowwise.
 * https://oeis.org/A235961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   diff_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A235961: Counts (n+1)×8 binary matrices whose 2x2 block
 * difference pattern forms a matrix that is
 *   * row‑wise nonincreasing  (each row is a block of 1’s followed by 0’s)
 *   * column‑wise nondecreasing (each column is a block of 0’s followed by 1’s).
 * The algorithm DP‑s over row masks, keeping track of the current
 * prefix‑ones length of the previous difference row. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/diff_mask.h"

#define ROWS   256      /* 8‑bit row masks */
#define MASKS  8        /* 0..7 one‐s prefix length */
#define TRANS_ROWS 256  /* number of possible previous rows */

/* Transition data: for every previous row, store all next rows that
 * produce a valid difference mask and the associated prefix length. */
struct Trans {
    unsigned char r_next;
    unsigned char k;      /* leading 1 length of the diff mask (0..7) */
};

static struct Trans trans[TRANS_ROWS][ROWS];
static unsigned char trans_cnt[TRANS_ROWS];
static unsigned char good_mask[1 << 7];
static unsigned char lead_len[1 << 7];
static bool tables_built = false;

/* Build transition tables once. */
static void init_tables(void)
{
    if (tables_built) return;

    /* Precompute good masks and their leading‑ones length. */
    for (uint32_t m = 0; m < (1 << 7); ++m) {
        bool ok = true;
        bool seen0 = false;
        unsigned cnt = 0;
        for (int j = 0; j < 7; ++j) {
            bool bit = (m >> j) & 1;
            if (bit) {
                if (seen0) { ok = false; break; }
                ++cnt;
            } else {
                seen0 = true;
            }
        }
        good_mask[m] = ok ? 1 : 0;
        lead_len[m]  = (unsigned char)cnt;
    }

    /* Build transitions for each previous row. */
    for (unsigned r0 = 0; r0 < ROWS; ++r0) {
        unsigned char cnt = 0;
        for (unsigned r1 = 0; r1 < ROWS; ++r1) {
            unsigned char m = diff_mask((unsigned char)r0, (unsigned char)r1);
            if (good_mask[m]) {
                trans[r0][cnt].r_next = (unsigned char)r1;
                trans[r0][cnt].k      = lead_len[m];
                ++cnt;
            }
        }
        trans_cnt[r0] = cnt;
    }

    tables_built = true;
}

/* DP arrays: dp[current][row][k]. */
static seq_t dp_cur[ROWS][MASKS];
static seq_t dp_nxt[ROWS][MASKS];

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset handling */

    init_tables();

    /* Initial state: choose any first row, previous k = 0. */
    memset(dp_cur, 0, sizeof(dp_cur));
    for (unsigned r = 0; r < ROWS; ++r) dp_cur[r][0] = 1;

    /* Perform n transitions (n+1 rows). */
    for (idx_t step = 0; step < n; ++step) {
        /* Prefix sums over k for each previous row. */
        seq_t pref[ROWS][MASKS];
        for (unsigned r = 0; r < ROWS; ++r) {
            pref[r][0] = dp_cur[r][0];
            for (unsigned k = 1; k < MASKS; ++k)
                pref[r][k] = pref[r][k - 1] + dp_cur[r][k];
        }

        /* Zero next DP array. */
        memset(dp_nxt, 0, sizeof(dp_nxt));

        /* Process transitions. */
        for (unsigned r0 = 0; r0 < ROWS; ++r0) {
            for (unsigned t = 0; t < trans_cnt[r0]; ++t) {
                unsigned char r1 = trans[r0][t].r_next;
                unsigned char k  = trans[r0][t].k;
                dp_nxt[r1][k] += pref[r0][k];
            }
        }

        /* Copy next to current for the next step. */
        memcpy(dp_cur, dp_nxt, sizeof(dp_cur));
    }

    /* Sum all final states. */
    seq_t result = 0;
    for (unsigned r = 0; r < ROWS; ++r)
        for (unsigned k = 0; k < MASKS; ++k)
            result += dp_cur[r][k];

    return result;
}
