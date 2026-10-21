/*
 * A206880  Number of nX5 0..1 arrays avoiding 0 1 0 horizontally and 1 0 1
 *          vertically.
 * https://oeis.org/A206880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   lowbit_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A206880: number of n×5 0..1 arrays avoiding
   0 1 0 horizontally and 1 0 1 vertically.
   Computation by dynamic programming over pairs of rows.
   For n=1 return count of a single row (21).
   For n≥2 initialize DP for all allowed row pairs (441),
   then for each further row apply vertical and horizontal
   constraints.  Sum all DP states for the answer. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/lowbit_index.h"

/* Size of a row in bits and number of possible rows */
#define ROWS   32          /* 2^5 */
#define STATES (ROWS * ROWS)

/* Pre‑computed tables (built once) */
static bool horiz_ok[ROWS];                /* row has no horizontal 010 */
static uint32_t trans_mask[ROWS][ROWS];    /* allowed next rows for (a,b) */

/* Flag whether tables are ready */
static bool tables_built = false;

/* Build tables of allowed rows and transitions */
static void build_tables(void)
{
    if (tables_built) return;
    tables_built = true;

    /* Fill horiz_ok: row may not contain 0 1 0 */
    for (int r = 0; r < ROWS; ++r) {
        bool ok = true;
        for (int c = 0; c < 3; ++c) {                 /* columns 0..2 */
            int b0 = (r >> c)     & 1;
            int b1 = (r >> (c+1)) & 1;
            int b2 = (r >> (c+2)) & 1;
            if (b0 == 0 && b1 == 1 && b2 == 0) { ok = false; break; }
        }
        horiz_ok[r] = ok;
    }

    /* Fill trans_mask: from (a,b) to next row r,
       ensure a,b,r all satisfy horizontal rule
       and no vertical 1 0 1 in any column among a,b,r. */
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            uint32_t mask = 0;
            if (!horiz_ok[a] || !horiz_ok[b]) { trans_mask[a][b] = 0; continue; }
            for (int r = 0; r < ROWS; ++r) {
                if (!horiz_ok[r]) continue;
                bool ok = true;
                for (int c = 0; c < 5; ++c) {
                    int da = (a >> c) & 1;
                    int db = (b >> c) & 1;
                    int dr = (r >> c) & 1;
                    if (da == 1 && db == 0 && dr == 1) { ok = false; break; }
                }
                if (ok) mask |= 1u << r;
            }
            trans_mask[a][b] = mask;
        }
    }
}

/* Main function computing a(n). */
seq_t a(idx_t n)
{
    build_tables();

    if (n == 0) return 0;          /* undefined, but safe */

    /* Count of single rows with no horizontal 010 */
    static seq_t single_row_count = 0;
    if (single_row_count == 0) {
        seq_t sum = 0;
        for (int r = 0; r < ROWS; ++r) if (horiz_ok[r]) ++sum;
        single_row_count = sum;
    }

    if (n == 1) return single_row_count;

    /* DP buffers for states (a,b) */
    static seq_t dp[STATES];
    static seq_t next_dp[STATES];

    /* reset dp */
    for (int i = 0; i < STATES; ++i) dp[i] = 0;

    /* initialize with all allowed pairs (n=2) */
    for (int a = 0; a < ROWS; ++a) {
        if (!horiz_ok[a]) continue;
        for (int b = 0; b < ROWS; ++b) {
            if (!horiz_ok[b]) continue;
            dp[(a << 5) | b] = (seq_t)1;
        }
    }

    /* Add remaining rows: from row 3 up to n */
    for (idx_t step = 2; step < n; ++step) {
        /* zero next_dp */
        for (int i = 0; i < STATES; ++i) next_dp[i] = 0;

        for (int idx = 0; idx < STATES; ++idx) {
            seq_t cur = dp[idx];
            if (cur == 0) continue;
            int a = idx >> 5;
            int b = idx & 31;
            uint32_t mask = trans_mask[a][b];
            while (mask) {
                int r = lowbit_index(mask);
                next_dp[(b << 5) | r] += cur;
                mask &= mask - 1;          /* remove lowest set bit */
            }
        }
        /* copy next_dp into dp */
        for (int i = 0; i < STATES; ++i) dp[i] = next_dp[i];
    }

    /* Sum all DP states for final count */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i) total += dp[i];
    return total;
}
