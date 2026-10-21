/*
 * A208312  Number of n X 6 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward neighbors.
 * https://oeis.org/A208312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208312: number of n×6 binary arrays with constraints  
   (new values 0..1 introduced in row major order, no element equal
   to more than one of its immediate leftward or upward neighbors).
   Computed by dynamic programming over row patterns.  */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static bool _a208312_comp[64][64];
static bool _a208312_comp_inited = false;

/* Precompute compatibility table:
   Transitions from prev row pattern to cur row pattern. */
static void _a208312_comp_init(void)
{
    if (_a208312_comp_inited)
        return;
    for (int prev = 0; prev < 64; ++prev) {
        for (int cur = 0; cur < 64; ++cur) {
            bool ok = true;
            for (int j = 1; j < 6; ++j) {          /* columns 1..5 (zero‑based) */
                int left  = (cur >> (j - 1)) & 1;   /* left neighbor in current row */
                int up    = (prev >> j) & 1;        /* upward neighbor */
                if (left == up) {                  /* both neighbors equal */
                    int curbit = (cur >> j) & 1;
                    if (curbit == left) {          /* would equal both neighbours */
                        ok = false;
                        break;
                    }
                }
            }
            _a208312_comp[prev][cur] = ok;
        }
    }
    _a208312_comp_inited = true;
}

seq_t a(idx_t n)
{
    /* If n is zero or negative (outside the offset), return 0. */
    if (n < (idx_t)1) {
        return (seq_t)0;
    }

    _a208312_comp_init();

    /* DP arrays for counts of ways ending with a particular row pattern. */
    static seq_t dp_prev[64];
    static seq_t dp_cur[64];

    /* Row 1: first element must be 0, all other bits free. */
    for (int r = 0; r < 64; ++r) {
        dp_prev[r] = ((r & 1) == 0) ? (seq_t)1 : (seq_t)0;
    }

    for (idx_t row = 2; row <= n; ++row) {
        memset(dp_cur, 0, sizeof(dp_cur));
        for (int prev = 0; prev < 64; ++prev) {
            seq_t ways = dp_prev[prev];
            if (!ways)
                continue;
            for (int cur = 0; cur < 64; ++cur) {
                if (_a208312_comp[prev][cur]) {
                    dp_cur[cur] += ways;
                }
            }
        }
        memcpy(dp_prev, dp_cur, sizeof(dp_prev));
    }

    /* Sum over all ending row patterns to get total arrays. */
    seq_t total = 0;
    for (int r = 0; r < 64; ++r) {
        total += dp_prev[r];
    }
    return total;
}
