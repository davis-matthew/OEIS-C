/*
 * A208196  Number of n X 3 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of n×3 arrays over {0,1,2,3} satisfying (i) new values
 *            appear in row‑major order (no value may appear before all
 *            smaller values have appeared) (ii) no element equals more than
 *            two of its left/up/left‑up neighbors. Offset: 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   validate_first_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/validate_first_row.h"

/* Transition table: for each (prevmax, prevrow) store all
   admissible (currow, newmax) pairs. */
typedef struct {
    unsigned short curRow;
    unsigned char newMax;
} trans_t;

static trans_t trans[256][64];
static unsigned char transCnt[256];
static bool trans_init = false;

/* Validate a row given the preceding row and current maximum. */
static bool validate_row(unsigned short prevRow, int prevMax,
                         unsigned short row, int *newmax) {
    int curmax = prevMax;
    for (int j = 0; j < 3; ++j) {
        int val = (row >> (2 * j)) & 3;
        if (val > curmax && val != curmax + 1)
            return false;
        if (val > curmax)
            curmax = val;
        int equalCnt = 0;
        if (j > 0) {
            int left = (row >> (2 * (j - 1))) & 3;
            if (left == val)
                ++equalCnt;
        }
        int up = (prevRow >> (2 * j)) & 3;
        if (up == val)
            ++equalCnt;
        if (j > 0) {
            int lup = (prevRow >> (2 * (j - 1))) & 3;
            if (lup == val)
                ++equalCnt;
        }
        if (equalCnt > 2)
            return false;
    }
    *newmax = curmax;
    return true;
}

/* Transition table initialization. */
static void init_transitions(void) {
    if (trans_init)
        return;
    for (int prevMax = 0; prevMax <= 3; ++prevMax) {
        for (int prevRow = 0; prevRow < 64; ++prevRow) {
            int idx = prevMax * 64 + prevRow;
            int cnt = 0;
            for (int row = 0; row < 64; ++row) {
                int newmax;
                if (validate_row(prevRow, prevMax, row, &newmax)) {
                    trans[idx][cnt].curRow = (unsigned short)row;
                    trans[idx][cnt].newMax = (unsigned char)newmax;
                    ++cnt;
                }
            }
            transCnt[idx] = (unsigned char)cnt;
        }
    }
    trans_init = true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;        /* not defined, but safe return */

    init_transitions();

    /* DP arrays: dp[cur][max][row] */
    static seq_t dp[2][4][64];
    int cur = 0, next = 1;
    for (int m = 0; m < 4; ++m)
        for (int r = 0; r < 64; ++r)
            dp[cur][m][r] = (seq_t)0;

    /* First row */
    for (int row = 0; row < 64; ++row) {
        int newmax;
        if (validate_first_row((unsigned short)row, &newmax)) {
            dp[cur][newmax][row] += (seq_t)1;
        }
    }

    /* Subsequent rows */
    for (idx_t rowIdx = 2; rowIdx <= n; ++rowIdx) {
        /* clear next DP */
        for (int m = 0; m < 4; ++m)
            for (int r = 0; r < 64; ++r)
                dp[next][m][r] = (seq_t)0;
        for (int prevMax = 0; prevMax <= 3; ++prevMax) {
            for (int prevRow = 0; prevRow < 64; ++prevRow) {
                seq_t ways = dp[cur][prevMax][prevRow];
                if (!ways)
                    continue;
                int stateIdx = prevMax * 64 + prevRow;
                for (int t = 0; t < transCnt[stateIdx]; ++t) {
                    const trans_t *tr = &trans[stateIdx][t];
                    dp[next][tr->newMax][tr->curRow] += ways;
                }
            }
        }
        int tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum all states */
    seq_t result = (seq_t)0;
    for (int m = 0; m <= 3; ++m)
        for (int r = 0; r < 64; ++r)
            result += dp[cur][m][r];

    return result;
}
