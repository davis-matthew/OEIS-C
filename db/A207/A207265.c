/*
 * A207265  Number of n X 4 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 0/1 arrays avoiding 000 and 011
 *            horizontally and 011 and 101 vertically
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 45, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define ROWS 16
#define STATE (ROWS * ROWS)

static unsigned char next_rows[STATE][ROWS];
static unsigned char next_counts[STATE];
static int trans_built = 0;

seq_t a(idx_t n)
{
    /* build once the list of valid rows and the transition table */
    static bool row_valid[ROWS];
    static int  valid_count = 0;

    if (!trans_built) {
        int v = 0;
        for (int p = 0; p < ROWS; ++p) {
            bool ok = true;
            /* check the two horizontal windows of length 3 */
            for (int w = 0; w <= 1; ++w) {
                int bits = (p >> w) & 0b111;
                if (bits == 0 || bits == 3) { ok = false; break; }
            }
            row_valid[p] = ok;
            if (ok) ++v;
        }
        valid_count = v;

        /* transition table: from (p,q) to all admissible r */
        for (int pi = 0; pi < ROWS; ++pi) {
            for (int qi = 0; qi < ROWS; ++qi) {
                int idx = pi * ROWS + qi;
                if (!row_valid[pi] || !row_valid[qi]) {
                    next_counts[idx] = 0;
                    continue;
                }
                int cnt = 0;
                for (int ri = 0; ri < ROWS; ++ri) {
                    if (!row_valid[ri]) continue;
                    bool vert_ok = true;
                    for (int j = 0; j < 4; ++j) {
                        int bp = (pi >> j) & 1;
                        int bq = (qi >> j) & 1;
                        int br = (ri >> j) & 1;
                        if ((bp == 0 && bq == 1 && br == 1) ||
                            (bp == 1 && bq == 0 && br == 1)) {
                            vert_ok = false;
                            break;
                        }
                    }
                    if (vert_ok) {
                        next_rows[idx][cnt++] = (unsigned char)ri;
                    }
                }
                next_counts[idx] = (unsigned char)cnt;
            }
        }
        trans_built = 1;
    }

    /* trivial cases */
    if (n == 1) return (seq_t)valid_count;
    if (n == 2) return (seq_t)valid_count * (seq_t)valid_count;

    /* DP arrays */
    seq_t dp[STATE]     = {0};
    seq_t next_dp[STATE] = {0};

    /* initialise with all first two rows */
    for (int pi = 0; pi < ROWS; ++pi) {
        if (!row_valid[pi]) continue;
        for (int qi = 0; qi < ROWS; ++qi) {
            if (!row_valid[qi]) continue;
            dp[pi * ROWS + qi] = (seq_t)1;
        }
    }

    idx_t steps = n - 2;          /* number of additional rows */
    for (idx_t step = 0; step < steps; ++step) {
        /* clear next_dp */
        for (int i = 0; i < STATE; ++i) next_dp[i] = (seq_t)0;

        /* transition */
        for (int idx = 0; idx < STATE; ++idx) {
            seq_t cnt = dp[idx];
            if (cnt == (seq_t)0) continue;
            int nb = next_counts[idx];
            for (int k = 0; k < nb; ++k) {
                unsigned char r = next_rows[idx][k];
                int new_idx = (idx % ROWS) * ROWS + r;  /* (q, r) */
                next_dp[new_idx] += cnt;
            }
        }

        /* move */
        for (int i = 0; i < STATE; ++i) dp[i] = next_dp[i];
    }

    /* sum over all states for the final count */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATE; ++i) total += dp[i];
    return total;
}
