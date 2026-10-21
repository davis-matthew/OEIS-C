/*
 * A231742  Number of nX3 0..3 arrays with no element less than a strict
 *          majority of its horizontal and vertical neighbors.
 * https://oeis.org/A231742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2,3} in which no element
 *            is less than a strict majority of its horizontal and vertical
 *            neighbors
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ROWS 64          /* 4^3 possible rows of length 3 */
#define STATE_COUNT (NUM_ROWS * NUM_ROWS)

static bool   _seqa_initialized = false;
static int    _rowVals[NUM_ROWS][3];
static uint64_t _allowedMask[STATE_COUNT];

/* ------------------------------------------------------------------ */
/* Helper: check validity of a row 'cur' with neighbors 'up' and
   'down'.   hasUp/dn specify presence of up/down neighbors.           */
static bool _row_valid(int curIdx, int upIdx, bool hasUp,
                       int downIdx, bool hasDown)
{
    for (int j = 0; j < 3; ++j) {
        int v = _rowVals[curIdx][j];
        int greater = 0;
        int neigh = 0;

        /* left */
        if (j > 0) {
            ++neigh;
            if (_rowVals[curIdx][j - 1] > v) ++greater;
        }
        /* right */
        if (j < 2) {
            ++neigh;
            if (_rowVals[curIdx][j + 1] > v) ++greater;
        }
        /* up */
        if (hasUp) {
            ++neigh;
            if (_rowVals[upIdx][j] > v) ++greater;
        }
        /* down */
        if (hasDown) {
            ++neigh;
            if (_rowVals[downIdx][j] > v) ++greater;
        }
        if (greater > neigh / 2) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Initialization: precompute row values and transition masks        */
static void _seqa_init(void)
{
    for (int r = 0; r < NUM_ROWS; ++r) {
        int t = r;
        _rowVals[r][0] = t % 4; t /= 4;
        _rowVals[r][1] = t % 4; t /= 4;
        _rowVals[r][2] = t % 4;
    }

    for (int up = 0; up < NUM_ROWS; ++up) {
        for (int cur = 0; cur < NUM_ROWS; ++cur) {
            uint64_t mask = 0;
            for (int down = 0; down < NUM_ROWS; ++down) {
                if (_row_valid(cur, up, true, down, true))
                    mask |= (1ULL << down);
            }
            _allowedMask[(up << 6) | cur] = mask;
        }
    }
    _seqa_initialized = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
seq_t a(idx_t n)
{
    if (! _seqa_initialized) _seqa_init();

    if (n == 0) return 0;          /* offset is 1; not used */

    /* n == 1: only one row, no vertical neighbours */
    if (n == 1) {
        seq_t sum = 0;
        for (int r = 0; r < NUM_ROWS; ++r)
            if (_row_valid(r, -1, false, -1, false))
                sum += 1;
        return sum;
    }

    /* Allocate DP arrays */
    seq_t *dp   = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
    if (!dp || !next) return 0;    /* memory exhaustion -> 0 */

    /* Initialisation for first two rows */
    for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
        for (int r2 = 0; r2 < NUM_ROWS; ++r2) {
            if (_row_valid(r1, -1, false, r2, true))
                dp[(r1 << 6) | r2] += 1;
        }
    }

    /* Add subsequent rows */
    for (idx_t step = 3; step <= n; ++step) {
        memset(next, 0, sizeof(seq_t) * STATE_COUNT);
        for (int idx = 0; idx < STATE_COUNT; ++idx) {
            seq_t cnt = dp[idx];
            if (!cnt) continue;
            uint64_t mask = _allowedMask[idx];
            int cur = idx & 63;
            while (mask) {
                uint64_t bit = mask & -mask;
                int down = __builtin_ctzll(bit);
                next[(cur << 6) | down] += cnt;
                mask -= bit;
            }
        }
        /* swap */
        seq_t *tmp = dp; dp = next; next = tmp;
    }

    /* Final row must satisfy constraints with no down neighbour */
    seq_t ans = 0;
    for (int idx = 0; idx < STATE_COUNT; ++idx) {
        seq_t cnt = dp[idx];
        if (!cnt) continue;
        int up  = idx >> 6;
        int cur = idx & 63;
        if (_row_valid(cur, up, true, -1, false))
            ans += cnt;
    }

    free(dp);
    free(next);
    return ans;
}
