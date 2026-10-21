/*
 * A234032  Number of (n+1)X(4+1) 0..4 arrays with every 2X2 subblock having
 *          the absolute values of all six edge and diagonal differences no
 *          larger than 1.
 * https://oeis.org/A234032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A234032 via dynamic programming over rows.
   The algorithm enumerates all 5‑length rows over {0..4} whose
   adjacent columns differ by at most 1.  It precomputes for each row
   the set of compatible rows (the pair of rows form a legal 2×2
   block), then uses a simple DP over the rows to obtain the number
   of (n+1)×5 grids for any n ≥ 1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define MAX_ROWS 3125          /* 5^5 = 3125, sufficient for all rows */

static int rows[MAX_ROWS][5];
static size_t comp_start[MAX_ROWS + 1];
static size_t comp_list[MAX_ROWS * MAX_ROWS];
static size_t num_rows = 0;
static bool tables_ready = false;

static void init_tables(void)
{
    if (tables_ready) return;

    /* enumerate all admissible rows */
    size_t idx = 0;
    for (int a = 0; a < 5; ++a)
        for (int b = 0; b < 5; ++b)
            if ((a - b) <= 1 && (b - a) <= 1)
                for (int c = 0; c < 5; ++c)
                    if ((b - c) <= 1 && (c - b) <= 1)
                        for (int d = 0; d < 5; ++d)
                            if ((c - d) <= 1 && (d - c) <= 1)
                                for (int e = 0; e < 5; ++e)
                                    if ((d - e) <= 1 && (e - d) <= 1) {
                                        rows[idx][0] = a;
                                        rows[idx][1] = b;
                                        rows[idx][2] = c;
                                        rows[idx][3] = d;
                                        rows[idx][4] = e;
                                        ++idx;
                                    }
    num_rows = idx;

    /* pre‑compute compatibility lists */
    size_t edge_idx = 0;
    comp_start[0] = 0;
    for (size_t i = 0; i < num_rows; ++i) {
        for (size_t j = 0; j < num_rows; ++j) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) {
                int a = rows[i][k];
                int b = rows[i][k + 1];
                int c = rows[j][k];
                int d = rows[j][k + 1];
                int m = a, M = a;
                if (b < m) m = b;
                if (c < m) m = c;
                if (d < m) m = d;
                if (b > M) M = b;
                if (c > M) M = c;
                if (d > M) M = d;
                if (M - m > 1) { ok = false; break; }
            }
            if (ok) comp_list[edge_idx++] = j;
        }
        comp_start[i + 1] = edge_idx;
    }

    tables_ready = true;
}

seq_t a(idx_t n)
{
    init_tables();
    if (n == 0) return 0;          /* undefined for the offset, but harmless */

    static seq_t dp0[MAX_ROWS];
    static seq_t dp1[MAX_ROWS];
    seq_t *cur = dp0;
    seq_t *nxt = dp1;

    /* initialize counts for the first row */
    for (size_t i = 0; i < num_rows; ++i) cur[i] = 1;

    /* perform n transitions to obtain n+1 rows */
    for (idx_t step = 1; step <= n; ++step) {
        /* zero next array */
        for (size_t i = 0; i < num_rows; ++i) nxt[i] = 0;

        /* propagate counts to compatible rows */
        for (size_t i = 0; i < num_rows; ++i) {
            seq_t val = cur[i];
            if (!val) continue;
            size_t start = comp_start[i];
            size_t end   = comp_start[i + 1];
            for (size_t p = start; p < end; ++p) {
                nxt[comp_list[p]] += val;
            }
        }

        /* swap arrays for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum counts for all possible last rows */
    seq_t result = 0;
    for (size_t i = 0; i < num_rows; ++i) result += cur[i];
    return result;
}
