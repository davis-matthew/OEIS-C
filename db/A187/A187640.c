/*
 * A187640  Number of (n+2)X3 0..2 arrays with each 3X3 subblock having sum 9.
 * https://oeis.org/A187640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of (n+2)×3 arrays with entries 0..2
 * such that every 3×3 subblock has sum 9.
 * The computation uses a DP over the sums of the last two rows,
 * exploiting that the 3×3 constraint depends only on the row sums.
 *
 * Let s(t) be the sum of row t (0 ≤ s(t) ≤ 6).  For each pair of
 * consecutive row sums (s(k), s(k+1)) the next row sum s(k+2)
 * is forced to be 9 − s(k) − s(k+1).  The number of rows with a
 * given sum s is
 *   sumcnt[0..6] = {1, 3, 6, 7, 6, 3, 1}
 * (counting solutions to x0+x1+x2=s, 0 ≤ xi ≤ 2).
 *
 * DP state: f[prev][curr] = number of ways to build the
 * already processed rows with last two row sums = prev, curr.
 *
 * Recurrence:
 *   f_next[curr][next] += f[prev][curr] * sumcnt[next]
 *   where next = 9 − prev − curr (valid only if 0 ≤ next ≤ 6).
 *
 * Initialise with first two rows (no constraint yet):
 *   f[sp][sc] = sumcnt[sp] * sumcnt[sc].
 *
 * After processing all N = n+2 rows, the answer is
 *   sum_{sp,sc} f[sp][sc].
 *
 * The algorithm runs in O(n·7²) time and O(7²) memory and works
 * for any build that defines seq_t and idx_t appropriately.
 */

#include <stddef.h>

const seq_t sumcnt[7] = {1, 3, 6, 7, 6, 3, 1};

seq_t a(idx_t n)
{
    /* Number of rows = n + 2 */
    idx_t N = n + 2;
    /* DP arrays: 7×7 matrices */
    seq_t cur[7][7] = {0};
    seq_t nxt[7][7] = {0};

    /* initial two rows: no constraints applied yet */
    for (idx_t sp = 0; sp <= 6; ++sp) {
        for (idx_t sc = 0; sc <= 6; ++sc) {
            cur[sp][sc] = sumcnt[sp] * sumcnt[sc];
        }
    }

    /* Process remaining rows, starting from the 3rd row (i = 3) */
    for (idx_t i = 3; i <= N; ++i) {
        /* clear next matrix */
        for (idx_t r = 0; r < 7; ++r)
            for (idx_t c = 0; c < 7; ++c)
                nxt[r][c] = 0;

        for (idx_t sp = 0; sp <= 6; ++sp) {
            for (idx_t sc = 0; sc <= 6; ++sc) {
                seq_t val = cur[sp][sc];
                if (val == 0) continue;
                idx_t sn = 9 - sp - sc;
                if (sn > 6) continue; /* invalid next sum */
                nxt[sc][sn] += val * sumcnt[sn];
            }
        }

        /* copy nxt to cur for next iteration */
        for (idx_t r = 0; r <= 6; ++r)
            for (idx_t c = 0; c <= 6; ++c)
                cur[r][c] = nxt[r][c];
    }

    /* sum all ending states */
    seq_t total = 0;
    for (idx_t sp = 0; sp <= 6; ++sp)
        for (idx_t sc = 0; sc <= 6; ++sc)
            total += cur[sp][sc];

    return total;
}
