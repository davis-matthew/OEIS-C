/*
 * A202313  Number of (n+2)X6 binary arrays avoiding patterns 001 and 100 in
 *          rows and columns.
 * https://oeis.org/A202313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of (n+2)×6 binary arrays avoiding patterns 001 and 100
   in rows and columns.
   We compute the sequence with a transfer‑matrix DP over the last
   two rows of the matrix.  Each row is a 6‑bit pattern that must
   itself avoid the forbidden triples.  When a new row is added
   the triple (a,b,c) in every column, where a,b are the two
   previous rows and c is the new row bit, must also avoid the
   forbidden triples.  With 6 bits each row there are 64 possible
   rows, hence 64²=4096 possible states for two consecutive rows.
   The transitions between states are pre‑computed once, after
   which the vector of counts is updated (H‑2) times where
   H=n+2 is the total height of the matrix.  Finally the
   counts of all states are summed to obtain a(n).  The
   implementation works for any unsigned integer type
   seq_t (including arbitrary‑precision ones) and follows the
   required interface. */

#include <stdint.h>
#include <stdbool.h>

#define STATE_COUNT 4096
#define ROW_COUNT 64
#define MAX_TRANS 64

static bool _init_done = false;
static bool _valid_row[ROW_COUNT];
static unsigned _trans_len[STATE_COUNT];
static unsigned _trans_dest[STATE_COUNT * MAX_TRANS];

static void _prepare(void)
{
    /* rows that themselves contain no 001 or 100 */
    for (int r = 0; r < ROW_COUNT; ++r) {
        bool ok = true;
        for (int j = 0; j + 2 < 6; ++j) {
            int a = (r >> j) & 1;
            int b = (r >> (j + 1)) & 1;
            int c = (r >> (j + 2)) & 1;
            if ((a == 0 && b == 0 && c == 1) ||
                (a == 1 && b == 0 && c == 0)) {
                ok = false;
                break;
            }
        }
        _valid_row[r] = ok;
    }

    /* pre‑compute transitions for each state (prev0, prev1) */
    for (int prev0 = 0; prev0 < ROW_COUNT; ++prev0) {
        if (!_valid_row[prev0]) continue;
        for (int prev1 = 0; prev1 < ROW_COUNT; ++prev1) {
            if (!_valid_row[prev1]) continue;
            int state = (prev0 << 6) | prev1;
            unsigned len = 0;
            for (int newrow = 0; newrow < ROW_COUNT; ++newrow) {
                if (!_valid_row[newrow]) continue;
                bool ok = true;
                for (int j = 0; j < 6; ++j) {
                    int a = (prev0 >> j) & 1;
                    int b = (prev1 >> j) & 1;
                    int c = (newrow >> j) & 1;
                    if ((a == 0 && b == 0 && c == 1) ||
                        (a == 1 && b == 0 && c == 0)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    _trans_dest[state * MAX_TRANS + len] = (prev1 << 6) | newrow;
                    len++;
                }
            }
            _trans_len[state] = len;
        }
    }

    _init_done = true;
}

seq_t a(idx_t n)
{
    const idx_t H = n + 2;            /* matrix height */
    const idx_t steps = H > 2 ? H - 2 : 0;  /* number of transitions */

    if (!_init_done) _prepare();

    /* vectors of counts for current and next DP layer */
    seq_t dp[STATE_COUNT];
    seq_t ndp[STATE_COUNT];

    /* initial state after two rows: all pairs of valid rows */
    for (int s = 0; s < STATE_COUNT; ++s) dp[s] = 0;
    for (int r0 = 0; r0 < ROW_COUNT; ++r0) {
        if (!_valid_row[r0]) continue;
        for (int r1 = 0; r1 < ROW_COUNT; ++r1) {
            if (!_valid_row[r1]) continue;
            int state = (r0 << 6) | r1;
            dp[state] = 1;
        }
    }

    /* iterate transitions */
    for (idx_t step = 0; step < steps; ++step) {
        for (int s = 0; s < STATE_COUNT; ++s) ndp[s] = 0;
        for (int s = 0; s < STATE_COUNT; ++s) {
            if (dp[s] == 0) continue;
            unsigned len = _trans_len[s];
            seq_t val = dp[s];
            unsigned base = s * MAX_TRANS;
            for (unsigned k = 0; k < len; ++k) {
                int dst = _trans_dest[base + k];
                ndp[dst] += val;
            }
        }
        /* swap dp and ndp */
        for (int s = 0; s < STATE_COUNT; ++s) dp[s] = ndp[s];
    }

    /* sum all final states */
    seq_t result = 0;
    for (int s = 0; s < STATE_COUNT; ++s) result += dp[s];
    return result;
}
