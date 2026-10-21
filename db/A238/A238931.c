/*
 * A238931  Number of (2+1)X(n+1) 0..3 arrays with no element greater than all
 *          horizontal neighbors or equal to all vertical neighbors.
 * https://oeis.org/A238931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3×(n+1) arrays with entries 0..3 such that for
 *            every element it is not greater than all horizontal neighbours
 *            (when only one neighbour exists it must not exceed that
 *            neighbour) and it is not equal to all vertical neighbours
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: seq_t and idx_t are provided by the build environment. */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t */
#include <stdbool.h>  /* for bool */

seq_t a(idx_t n)
{
    /* For the definition only n ≥ 1 is meaningful. */
    if (n == (idx_t)0) return (seq_t)0;

    const idx_t L = n + 1;      /* number of columns                */
    const int K = 36;           /* number of admissible columns     */

    /* ----- static data common to all calls ----- */
    static int col[36][3];              /* the 36 admissible columns      */
    static bool col_leq[36][36];        /* col[i] <= col[j] elementwise   */
    static uint64_t next_mask[36][36];  /* bitmask of allowable next cols */
    static bool init = false;
    if (!init) {
        init = true;

        /* generate all columns with t != m and m != b */
        int idx = 0;
        for (int t = 0; t < 4; ++t) {
            for (int m = 0; m < 4; ++m) if (m != t) {
                for (int b = 0; b < 4; ++b) if (b != m) {
                    col[idx][0] = t;
                    col[idx][1] = m;
                    col[idx][2] = b;
                    ++idx;
                }
            }
        }
        /* precompute element‑wise <= */
        for (int i = 0; i < K; ++i) {
            for (int j = 0; j < K; ++j) {
                bool ok = true;
                for (int r = 0; r < 3; ++r) {
                    if (col[i][r] > col[j][r]) { ok = false; break; }
                }
                col_leq[i][j] = ok;
            }
        }
        /* precompute allowed next columns for every (prevprev, prev) pair */
        for (int p = 0; p < K; ++p) {
            for (int q = 0; q < K; ++q) {
                uint64_t mask = 0;
                for (int r = 0; r < K; ++r) {
                    bool ok = true;
                    for (int row = 0; row < 3; ++row) {
                        int left  = col[p][row];
                        int mid   = col[q][row];
                        int right = col[r][row];
                        if (mid > left && mid > right) { ok = false; break; }
                    }
                    if (ok) mask |= ((uint64_t)1 << r);
                }
                next_mask[p][q] = mask;
            }
        }
    }

    /* ----- dynamic programming over columns ----- */
    static seq_t dp1[36][36];
    static seq_t dp2[36][36];

    /* initialise first two columns: col0 <= col1 */
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < K; ++j)
            dp1[i][j] = (col_leq[i][j]) ? (seq_t)1 : (seq_t)0;

    if (L >= 3) {
        idx_t steps = L - 2;   /* number of additional columns after the first two */
        for (idx_t s = 0; s < steps; ++s) {
            /* zero dp2 */
            for (int i = 0; i < K; ++i)
                for (int j = 0; j < K; ++j)
                    dp2[i][j] = (seq_t)0;

            for (int p = 0; p < K; ++p) {
                for (int q = 0; q < K; ++q) {
                    seq_t cur = dp1[p][q];
                    if (!cur) continue;
                    uint64_t mask = next_mask[p][q];
                    for (int r = 0; r < K; ++r) {
                        if (mask & ((uint64_t)1 << r))
                            dp2[q][r] += cur;
                    }
                }
            }
            /* copy dp2 back into dp1 for next iteration */
            for (int i = 0; i < K; ++i)
                for (int j = 0; j < K; ++j)
                    dp1[i][j] = dp2[i][j];
        }
    }

    /* sum over final states where last column <= previous column */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < K; ++i) {          /* previous column (c_{L-2}) */
        for (int j = 0; j < K; ++j) {      /* last column (c_{L-1})   */
            if (col_leq[j][i]) ans += dp1[i][j];
        }
    }
    return ans;
}
