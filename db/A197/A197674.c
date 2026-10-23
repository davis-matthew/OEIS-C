/*
 * A197674  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 2,0,1,0,1 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A197674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function computes the number of n×3 arrays * using digits
 *            0..4 such that each cell x has exactly x * neighbors equal to a
 *            value that depends on x: * 0 -> neighbors equal to 2 : 0 times *
 *            1 -> neighbors equal to 0 : 1 time * 2 -> neighbors equal to 1 :
 *            2 times * 3 -> neighbors equal to 0 : 3...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build environment. */

static const int TARGET[5] = {2, 0, 1, 0, 1};

/* All 125 possible row configurations, encoded as base‑5 digits. */
static int ROW[125][3];

/* Pre‑computed lists of admissible next rows for each pair of consecutive rows. */
#define MAX_ROWS 125
#define MAX_STATE (MAX_ROWS * MAX_ROWS)
#define MAX_TRANSITION 64
static unsigned char TRANS_CNT[MAX_STATE];
static int TRANS_NEXT[MAX_STATE][MAX_TRANSITION];

/* Helper: check whether a row configuration r is valid when
 * it has an optional up row (prev) and an optional down row (next).
 * If up_present/down_present is false, the corresponding neighbor is absent.
 */
static bool row_valid_mask(int r, const int *up, const int *next,
                           bool up_present, bool down_present)
{
    int *curr = ROW[r];
    for (int j = 0; j < 3; ++j) {
        int t = TARGET[curr[j]];
        int count = 0;
        if (j > 0 && curr[j-1] == t) ++count;
        if (j < 2 && curr[j+1] == t) ++count;
        if (up_present && up[j] == t) ++count;
        if (down_present && next[j] == t) ++count;
        if (count != curr[j]) return false;
    }
    return true;
}

/* Build the transition tables once. */
static void build_transitions(void)
{
    for (int i = 0; i < MAX_ROWS; ++i) {
        for (int j = 0; j < MAX_ROWS; ++j) {
            int idx = i * MAX_ROWS + j;
            unsigned char cnt = 0;
            for (int r = 0; r < MAX_ROWS; ++r) {
                if (!row_valid_mask(j, ROW[i], ROW[r], true, true))
                    continue;
                TRANS_NEXT[idx][cnt++] = r;
            }
            TRANS_CNT[idx] = cnt;
        }
    }
}

/* Return the number of n×3 arrays satisfying the constraints. */
seq_t a(idx_t n)
{
    if (!ROW[0][0]) { /* initialise on first call */
        for (int i = 0; i < MAX_ROWS; ++i) {
            int v = i;
            for (int j = 0; j < 3; ++j) {
                ROW[i][j] = v % 5;
                v /= 5;
            }
        }
        build_transitions();
    }

    /* Special cases n = 1 and n = 2 are handled separately. */
    if (n == 0) return 0;

    if (n == 1) {
        seq_t total = 0;
        for (int r = 0; r < MAX_ROWS; ++r)
            if (row_valid_mask(r, NULL, NULL, false, false))
                ++total;
        return total;
    }

    static seq_t dp_cur[MAX_STATE];
    static seq_t dp_next[MAX_STATE];
    /* initialise dp_cur with pairs (row1,row2) that make row1 valid
     * with up absent, down=row2. */
    for (int i = 0; i < MAX_STATE; ++i) dp_cur[i] = 0;
    for (int r1 = 0; r1 < MAX_ROWS; ++r1) {
        for (int r2 = 0; r2 < MAX_ROWS; ++r2) {
            if (!row_valid_mask(r1, NULL, ROW[r2], false, true))
                continue;
            dp_cur[r1 * MAX_ROWS + r2] = 1;
        }
    }

    if (n == 2) {
        seq_t total = 0;
        for (int idx = 0; idx < MAX_STATE; ++idx) {
            if (!dp_cur[idx]) continue;
            int rprev = idx / MAX_ROWS;
            int rcurr = idx % MAX_ROWS;
            if (row_valid_mask(rcurr, ROW[rprev], NULL, true, false))
                total += dp_cur[idx];
        }
        return total;
    }

    /* For n >= 3 iterate over rows 3..n. */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < MAX_STATE; ++i) dp_next[i] = 0;
        for (int idx = 0; idx < MAX_STATE; ++idx) {
            seq_t val = dp_cur[idx];
            if (!val) continue;
            unsigned char cnt = TRANS_CNT[idx];
            int *list = TRANS_NEXT[idx];
            for (unsigned char k = 0; k < cnt; ++k) {
                int rnext = list[k];
                int next_idx = (idx % MAX_ROWS) * MAX_ROWS + rnext;
                dp_next[next_idx] += val;
            }
        }
        /* swap dp arrays */
        for (int i = 0; i < MAX_STATE; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum over final row pairs where last row is valid with up=rprev,
       down absent. */
    seq_t total = 0;
    for (int idx = 0; idx < MAX_STATE; ++idx) {
        seq_t val = dp_cur[idx];
        if (!val) continue;
        int rprev = idx / MAX_ROWS;
        int rcurr = idx % MAX_ROWS;
        if (row_valid_mask(rcurr, ROW[rprev], NULL, true, false))
            total += val;
    }
    return total;
}
