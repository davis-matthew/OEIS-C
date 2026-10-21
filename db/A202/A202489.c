/*
 * A202489  Number of (n+2)X7 binary arrays avoiding patterns 000 and 010 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×7 binary arrays avoiding patterns 000 *
 *            and 010 in rows, columns and NW-to-SE diagonals. Computed via *
 *            dynamic programming on rows. * * The implementation precomputes
 *            all valid row masks (no 000 or 010 * horizontally) and, for
 *            every pair of previous rows, all possible next...
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   no_forbidden_triplet
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/no_forbidden_triplet.h"

#define COLS 7
#define MASKS (1 << COLS)

/* Precomputed row validity */
static bool row_valid[MASKS];
/* List of all masks that are valid rows */
static uint8_t valid_masks[64];
static uint8_t num_valid_masks;

/* Transition structure for a pair of previous rows */
typedef struct {
    uint8_t count;
    uint8_t next[64]; /* maximum transitions is <64 for 7 columns */
} Trans;
static Trans trans[MASKS][MASKS];

/* Precompute row validity and list of valid masks */
static void precompute_rows(void)
{
    num_valid_masks = 0;
    for (int mask = 0; mask < MASKS; ++mask) {
        bool ok = true;
        for (int i = 0; i <= COLS-3 && ok; ++i) {
            uint8_t b0 = (mask >> i)     & 1;
            uint8_t b1 = (mask >> (i+1)) & 1;
            uint8_t b2 = (mask >> (i+2)) & 1;
            if (b0==0 && b1==0 && b2==0) ok = false;
            if (b0==0 && b1==1 && b2==0) ok = false;
        }
        row_valid[mask] = ok;
        if (ok) {
            valid_masks[num_valid_masks++] = (uint8_t)mask;
        }
    }
}

/* Precompute transitions for all pairs of previous rows */
static void precompute_transitions(void)
{
    for (int p1 = 0; p1 < MASKS; ++p1) {
        for (int p2 = 0; p2 < MASKS; ++p2) {
            Trans *t = &trans[p1][p2];
            t->count = 0;
            for (int k = 0; k < num_valid_masks; ++k) {
                uint8_t cur = valid_masks[k];
                bool ok = true;
                /* Vertical check for columns 0..6 */
                for (int c = 0; c < COLS && ok; ++c) {
                    uint8_t b0 = (p2 >> c) & 1;
                    uint8_t b1 = (p1 >> c) & 1;
                    uint8_t b2 = (cur >> c) & 1;
                    if (!no_forbidden_triplet(b0, b1, b2)) ok = false;
                }
                /* NW‑SE diagonal check: columns 2..6 */
                for (int c = 2; c < COLS && ok; ++c) {
                    uint8_t b0 = (p2 >> (c-2)) & 1;
                    uint8_t b1 = (p1 >> (c-1)) & 1;
                    uint8_t b2 = (cur >> c) & 1;
                    if (!no_forbidden_triplet(b0, b1, b2)) ok = false;
                }
                if (ok) {
                    t->next[t->count++] = cur;
                }
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Number of rows = n + 2 */
    if (n == 0) return 0;

    precompute_rows();
    precompute_transitions();

    static seq_t dp[2][MASKS][MASKS];
    int cur = 0, nxt = 1;
    /* initialize dp arrays to zero */
    for (int i = 0; i < MASKS; ++i)
        for (int j = 0; j < MASKS; ++j)
            dp[cur][i][j] = 0, dp[nxt][i][j] = 0;

    /* Row 1 */
    for (int k = 0; k < num_valid_masks; ++k) {
        uint8_t m = valid_masks[k];
        dp[cur][m][0] = 1;
    }

    /* Row 2 */
    for (int prev = 0; prev < MASKS; ++prev) {
        seq_t cnt = dp[cur][prev][0];
        if (cnt == 0) continue;
        for (int k = 0; k < num_valid_masks; ++k) {
            uint8_t m = valid_masks[k];
            dp[nxt][m][prev] += cnt;
        }
    }
    int tmp = cur; cur = nxt; nxt = tmp;
    /* dp[cur] now holds DP after two rows */

    /* Rows 3 .. n+2 */
    for (idx_t r = 3; r <= n + 2; ++r) {
        /* zero next layer */
        for (int i = 0; i < MASKS; ++i)
            for (int j = 0; j < MASKS; ++j)
                dp[nxt][i][j] = 0;
        for (int p1 = 0; p1 < MASKS; ++p1) {
            for (int p2 = 0; p2 < MASKS; ++p2) {
                seq_t cnt = dp[cur][p1][p2];
                if (cnt == 0) continue;
                Trans *t = &trans[p1][p2];
                for (int tcnt = 0; tcnt < t->count; ++tcnt) {
                    uint8_t m = t->next[tcnt];
                    dp[nxt][m][p1] += cnt;
                }
            }
        }
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* Sum all counts */
    seq_t total = 0;
    for (int i = 0; i < MASKS; ++i)
        for (int j = 0; j < MASKS; ++j)
            total += dp[cur][i][j];

    return total;
}
