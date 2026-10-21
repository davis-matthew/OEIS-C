/*
 * A236037  Number of (n+1)X(1+1) 0..3 arrays colored with the difference of
 *          the upper median and the lower median in each 2X2 subblock.
 * https://oeis.org/A236037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays of 0..3 such that for each
 *            adjacent 2×2 subblock the difference (upper median – lower
 *            median) differs from that of the previous subblock. Computed by
 *            a 64‑state automaton; initial state vector counts all 2‑row
 *            arrays, transitions enforce distinct differences
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   diff_block
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/diff_block.h"

/* ------------------------------------------------------------------
 * Global tables (initialised once).
 * ------------------------------------------------------------------ */
#define STATES 64          /* 16 rows × 4 possible previous diffs */

static uint8_t  _diff[16][16];          /* diff between two rows   */
static uint8_t  _cnt[16][4];            /* number of predecessors for each row and diff */
static seq_t    _V0[STATES];           /* initial state vector    */
static seq_t    _a1;                   /* a(1) = 256              */
static uint8_t  _Tlist[STATES][16];    /* adjacency list          */
static uint8_t  _Tcnt[STATES];         /* number of successors    */
static bool     _init_done = false;

/* ------------------------------------------------------------------
 * Initialise all tables the first time the function is called.
 * ------------------------------------------------------------------ */
static void _init_once(void)
{
    if (_init_done) return;

    /* Build difference matrix */
    for (int r1 = 0; r1 < 16; ++r1) {
        for (int r2 = 0; r2 < 16; ++r2)
            _diff[r1][r2] = diff_block((uint8_t)r1, (uint8_t)r2);
    }

    /* Count predecessors for each row and diff value */
    for (int r = 0; r < 16; ++r)
        for (int d = 0; d < 4; ++d)
            _cnt[r][d] = 0;

    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            uint8_t d = _diff[prev][curr];
            _cnt[curr][d]++;                    /* one predecessor */
        }
    }

    /* Initial state vector for n = 1 (two rows only) */
    for (int curr = 0; curr < 16; ++curr)
        for (int d = 0; d < 4; ++d)
            _V0[curr * 4 + d] = (seq_t)_cnt[curr][d];

    /* a(1) is the total number of (2×2) arrays */
    _a1 = 0;
    for (int i = 0; i < STATES; ++i) _a1 += _V0[i];

    /* Build transition lists.
       From state (curr, d_prev) we can move to (next, d_next) provided
       d_next ≠ d_prev. */
    for (int curr = 0; curr < 16; ++curr) {
        for (int d_prev = 0; d_prev < 4; ++d_prev) {
            int from = curr * 4 + d_prev;
            _Tcnt[from] = 0;
            for (int next = 0; next < 16; ++next) {
                uint8_t d_next = _diff[curr][next];
                if (d_next == d_prev) continue;
                int to = next * 4 + d_next;
                _Tlist[from][_Tcnt[from]++] = (uint8_t)to;
            }
        }
    }

    _init_done = true;
}

/* ------------------------------------------------------------------
 * Main sequence function.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    _init_once();

    if (n <= 1) return _a1;          /* a(1) = 256, a(0) undefined */

    /* Prepare current state vector */
    seq_t vec[STATES];
    for (int i = 0; i < STATES; ++i) vec[i] = _V0[i];

    /* Perform n-1 transitions */
    idx_t steps = n - 1;
    seq_t next[STATES];

    for (idx_t s = 0; s < steps; ++s) {
        /* zero the next vector */
        for (int i = 0; i < STATES; ++i) next[i] = 0;

        /* propagate counts along allowed transitions */
        for (int i = 0; i < STATES; ++i) {
            seq_t cnt = vec[i];
            if (!cnt) continue;
            for (int k = 0; k < _Tcnt[i]; ++k) {
                int to = _Tlist[i][k];
                next[to] += cnt;
            }
        }

        /* copy next to current for the next iteration */
        for (int i = 0; i < STATES; ++i) vec[i] = next[i];
    }

    /* Sum all state counts to obtain a(n) */
    seq_t sum = 0;
    for (int i = 0; i < STATES; ++i) sum += vec[i];
    return sum;
}
