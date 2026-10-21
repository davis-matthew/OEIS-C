/*
 * A208160  Number of nX4 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A208160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by a transfer–matrix DP based on allowed row patterns
 *            and vertical constraints. The first 9 rows of the sequence are
 *            a(1)=9, a(2)=81, etc. The DP uses a 16×16 table of big integers
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   is_allowed_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0/1 rows of length 4 that avoid the forbidden horizontal patterns
   000 and 101 are called "allowed" rows.  Then each new row is
   admissible only if, together with the two previous rows, it does
   not contain the vertical forbidden patterns 001 or 010 in any
   column.  By counting all arrays with this property the sequence
   A208160 is obtained.  The DP runs in O(n·16³), well within the
   time limit for n≤10000. */

#include <stdbool.h>
#include "../utils/is_allowed_row.h"

/* Pre‑computed tables, initialised once. */
static bool init_done = false;
static bool allowed[16];
static unsigned char allowed_cnt;
static unsigned char trans_cnt[16][16];      /* number of legal next rows   */
static int trans_next[16][16][16];          /* list of legal next rows     */

/* Initialise allowed rows and transition tables. */
static void initialise_tables(void)
{
    if (init_done) return;
    init_done = true;

    allowed_cnt = 0;
    for (int p = 0; p < 16; p++) {
        if (is_allowed_row(p)) {
            allowed[p] = true;
            allowed_cnt++;
        } else {
            allowed[p] = false;
        }
    }

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            trans_cnt[i][j] = 0;
        }
    }

    for (int i = 0; i < 16; i++) {
        if (!allowed[i]) continue;
        for (int j = 0; j < 16; j++) {
            if (!allowed[j]) continue;
            for (int k = 0; k < 16; k++) {
                if (!allowed[k]) continue;
                bool ok = true;
                for (int col = 0; col < 4; col++) {
                    int b1 = (i >> (3 - col)) & 1;       /* row i */
                    int b2 = (j >> (3 - col)) & 1;       /* row j */
                    int b3 = (k >> (3 - col)) & 1;       /* row k */
                    if ((b1 == 0 && b2 == 0 && b3 == 1) ||
                        (b1 == 0 && b2 == 1 && b3 == 0)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    int idx = trans_cnt[i][j]++;
                    trans_next[i][j][idx] = k;
                }
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    initialise_tables();

    if (n == 0) return 0;                       /* unused, offset 1 */
    if (n == 1) return (seq_t)allowed_cnt;      /* single row */

    /* For n==2 all pairs of allowed rows are valid. */
    if (n == 2) {
        seq_t val = (seq_t)allowed_cnt;
        return val * val;
    }

    /* DP tables for last two rows. */
    static seq_t dp_prev[16][16];
    static seq_t dp_curr[16][16];

    /* Initialise dp_prev for n = 2 (any allowed pair). */
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            dp_prev[i][j] = 0;
            if (allowed[i] && allowed[j]) dp_prev[i][j] = 1;
        }
    }

    /* Build up from row 3 to n. */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear dp_curr */
        for (int i = 0; i < 16; i++) for (int j = 0; j < 16; j++) dp_curr[i][j] = 0;

        for (int p2 = 0; p2 < 16; p2++) if (allowed[p2]) {
            for (int p1 = 0; p1 < 16; p1++) if (allowed[p1]) {
                seq_t val = dp_prev[p2][p1];
                if (!val) continue;
                unsigned char c = trans_cnt[p2][p1];
                for (unsigned char t = 0; t < c; t++) {
                    int next = trans_next[p2][p1][t];
                    dp_curr[p1][next] += val;
                }
            }
        }

        /* Copy dp_curr into dp_prev for next iteration. */
        for (int i = 0; i < 16; i++)
            for (int j = 0; j < 16; j++)
                dp_prev[i][j] = dp_curr[i][j];
    }

    /* Sum all entries for the final row count. */
    seq_t total = 0;
    for (int i = 0; i < 16; i++) if (allowed[i]) {
        for (int j = 0; j < 16; j++) if (allowed[j]) {
            total += dp_prev[i][j];
        }
    }
    return total;
}
