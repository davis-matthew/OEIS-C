/*
 * A184759  Half the number of nX7 binary arrays with no 1 having an adjacent
 *          1 both above and to its left.
 * https://oeis.org/A184759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A184759: half the number of n×7 binary arrays
 * in which no entry 1 has a 1 immediately above it and immediately to its
 * left.  The algorithm uses dynamic programming over the 2^7 row masks
 * and pre‑computes the admissible transitions between rows.  The count
 * of valid matrices after n rows is obtained by iterating over the rows
 * and accumulating the number of ways to reach each mask.  Finally
 * the total count is divided by two as required. */
#include <stdbool.h>
#include <stdint.h>

/* Pre‑computed transition data between row masks (7 bits). */
static unsigned char neigh_cnt[128];
static unsigned char neighs[128][128];
static bool neighs_ready = false;

/* Helper: generate admissible transitions once. */
static void init_neighs(void)
{
    if (neighs_ready) return;
    for (int p = 0; p < 128; ++p) {
        int cnt = 0;
        for (int q = 0; q < 128; ++q) {
            int ok = 1;
            for (int j = 0; j < 7; ++j) {
                int bit = (q >> j) & 1;
                if (!bit) continue;
                int up   = (p >> j) & 1;          /* previous row */
                int left = (j > 0) ? ((q >> (j-1)) & 1) : 0; /* same row */
                if (up && left) { ok = 0; break; }
            }
            if (ok) {
                neighs[p][cnt++] = (unsigned char)q;
            }
        }
        neigh_cnt[p] = (unsigned char)cnt;
    }
    neighs_ready = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    init_neighs();

    seq_t dp[128]   = {0};
    seq_t next[128] = {0};

    dp[0] = 1;   /* previous row all zeros */

    for (idx_t row = 0; row < n; ++row) {
        /* zero next array */
        for (int i = 0; i < 128; ++i) next[i] = 0;

        /* transition */
        for (int p = 0; p < 128; ++p) {
            seq_t val = dp[p];
            if (!val) continue;
            int deg = neigh_cnt[p];
            for (int k = 0; k < deg; ++k) {
                int q = neighs[p][k];
                next[q] += val;
            }
        }

        /* swap dp and next */
        for (int i = 0; i < 128; ++i) dp[i] = next[i];
    }

    /* sum all final states */
    seq_t total = 0;
    for (int i = 0; i < 128; ++i) total += dp[i];

    /* the sequence is half that count */
    return total / 2;
}
