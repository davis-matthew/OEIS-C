/*
 * A081926  Triangle read by rows in which n-th row gives n smallest numbers
 *          with digit sum n.
 * https://oeis.org/A081926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 738, u32 n <= 3376, u64 n <= 10011,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 82 terms through n = 9261 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the pos‑th smallest integer with digit sum r,
 * where r is the unique integer satisfying
 *   r(r+1)/2 ≥ n > r(r-1)/2
 * and pos = n - r(r-1)/2.  Each row r consists of r smallest numbers
 * (in increasing order) having digit sum r.  The sequence is generated
 * by computing the required row lazily and caching the results. */

#include <stdbool.h>
#include <stdint.h>

/* Recursive helper that builds numbers of length l with the current
 * position `pos` (0‑based), remaining digit sum `rem`,
 * current numeric value `cur`, and fills into `row` until `target`
 * numbers are collected. */
static void dfs_gen(int l, int pos, idx_t rem, seq_t cur,
                    int *cnt, int target, seq_t *row)
{
    if (*cnt >= target) return;
    if (pos == l) {                 /* one candidate finished */
        if (rem == 0) {
            row[(*cnt)++] = cur;
        }
        return;
    }
    int min_d = (pos == 0) ? 1 : 0; /* first digit may not be zero */
    for (int d = min_d; d <= 9 && (idx_t)d <= rem; ++d) {
        seq_t next = cur * 10 + (seq_t)d;
        int remaining_positions = l - pos - 1;
        if ((idx_t)(rem - d) > (idx_t)(9 * remaining_positions)) continue;
        dfs_gen(l, pos + 1, rem - d, next, cnt, target, row);
        if (*cnt >= target) return;
    }
}

/* Maximum row we pre‑compute.  For n ≤ 10⁴ this is far more than needed. */
#define MAX_R 1000

/* Cache for the first r numbers of row r.  row_len[r] stores how many
 * values are in row_values[r][0 .. row_len[r]-1].  row_done[r] marks
 * whether row r has been filled. */
static seq_t   row_values[MAX_R + 1][MAX_R + 1];
static int     row_len[MAX_R + 1];
static bool    row_done[MAX_R + 1] = { false };

/* Fill the cache for row r   (assumes r ≤ MAX_R). */
static void compute_row(int r)
{
    int target = r;
    int l_min   = (r + 8) / 9;           /* ceil(r/9) – minimal possible length */
    int cnt     = 0;
    for (int l = l_min; cnt < target; ++l) {
        dfs_gen(l, 0, (idx_t)r, (seq_t)0,
                &cnt, target, row_values[r]);
    }
    row_done[r] = true;
    row_len[r]  = target;
}

/* Main function requested by the framework. */
seq_t a(idx_t n)
{
    /* Find r with r(r+1)/2 >= n > r(r-1)/2 using binary search. */
    uint64_t low  = 1, high = 1;
    while (high * (high + 1) / 2 < n) high <<= 1;   /* ensure high large enough */
    while (low < high) {
        uint64_t mid = (low + high) >> 1;
        if (mid * (mid + 1) / 2 >= n) high = mid;
        else low = mid + 1;
    }
    int r = (int)low;
    uint64_t prev = (uint64_t)r * (r - 1) / 2;
    int pos = (int)(n - prev);           /* 1‑based position in row r */

    if (!row_done[r]) compute_row(r);
    return row_values[r][pos - 1];
}
