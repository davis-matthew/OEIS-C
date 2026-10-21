/*
 * A240787  Number of 6Xn 0..1 arrays with no element equal to fewer vertical
 *          neighbors than horizontal neighbors, with new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A240787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A240787
    Number of 6×n 0..1 arrays with no element equal to fewer
    vertical neighbors than horizontal neighbors, with new values
    0..1 introduced in row major order.

    Implementation: dynamic programming over column patterns.
    The state consists of the two previous columns; there are at most
    65·64=4160 distinct states.  For each state the set of admissible
    next columns is pre‑computed.  The DP iterates over all N-1 column
    transitions, afterwards the last column is checked.  The final
    count is divided by 2 because arrays that are complementary
    (bitwise inversion) are considered identical.  The algorithm
    runs in O(N) time with a small constant factor. */

#include <stdbool.h>
#include <stdint.h>

#define NUM_ROWS   6
#define NUM_PAT    64               /* 2^6 patterns for a column */
#define SENTINEL   NUM_PAT           /* 64  – represents "no left column"  */
#define STATES     ((SENTINEL+1)*NUM_PAT)

/* Pre‑computed tables (built only once) */
static bool _initialized = false;
static uint8_t  _vertical[NUM_PAT][NUM_ROWS];      /* V[pat][row]  */
static uint64_t _mask[NUM_PAT+1][NUM_PAT];         /* allowed transitions */

/* ------------------------------------------------------------- */
/*   Helper to test whether the last column (pattern 'col')
    satisfies the horizontal constraint with only a left neighbour. */
static bool _valid_last_column(int pp, int p)
{
    if (pp == SENTINEL)                /* no left neighbour */
        return true;                   /* horizontal contribution is 0 */

    for (int r = 0; r < NUM_ROWS; ++r) {
        uint8_t val   = (p >> r) & 1;
        uint8_t left  = (((pp >> r) & 1) == val) ? 1 : 0;
        if (_vertical[p][r] < left)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* ---  initialise tables once -------------------------------- */
    if (!_initialized) {
        /* vertical neighbour counts for each column pattern */
        for (int p = 0; p < NUM_PAT; ++p) {
            for (int r = 0; r < NUM_ROWS; ++r) {
                uint8_t v = 0;
                uint8_t val = (p >> r) & 1;
                if (r > 0   && ((p >> (r-1)) & 1) == val) ++v;
                if (r < 5 && ((p >> (r+1)) & 1) == val) ++v;
                _vertical[p][r] = v;
            }
        }

        /* masks of admissible next columns for every state (pp,p) */
        for (int pp = 0; pp <= SENTINEL; ++pp) {
            for (int p = 0; p < NUM_PAT; ++p) {
                uint64_t mask = 0;
                for (int ncol = 0; ncol < NUM_PAT; ++ncol) {
                    bool ok = true;
                    for (int r = 0; r < NUM_ROWS && ok; ++r) {
                        uint8_t val = (p >> r) & 1;
                        uint8_t v   = _vertical[p][r];
                        uint8_t left  = (pp < SENTINEL &&
                                         ((pp >> r) & 1) == val) ? 1 : 0;
                        uint8_t right = (((ncol >> r) & 1) == val) ? 1 : 0;
                        int h = left + right;
                        if (v < h) ok = false;
                    }
                    if (ok) mask |= (1ULL << ncol);
                }
                _mask[pp][p] = mask;
            }
        }
        _initialized = true;
    }

    /* ---  DP arrays ------------------------------------------------ */
    static seq_t cur[STATES];
    static seq_t nxt[STATES];

    /* reset DP */
    for (int i = 0; i < STATES; ++i) {
        cur[i] = 0;
        nxt[i] = 0;
    }

    /* initial state: only the first column (sentinel, col) */
    for (int p = 0; p < NUM_PAT; ++p)
        cur[SENTINEL * NUM_PAT + p] = (seq_t)1;

    /* main DP loop over columns (col 1 … n-1) */
    for (int col = 1; col < (int)n; ++col) {
        /* clear next array */
        for (int i = 0; i < STATES; ++i) nxt[i] = 0;

        for (int pp = 0; pp <= SENTINEL; ++pp) {
            for (int p = 0; p < NUM_PAT; ++p) {
                seq_t val = cur[pp * NUM_PAT + p];
                if (val == 0) continue;

                uint64_t mask = _mask[pp][p];
                for (int ncol = 0; ncol < NUM_PAT; ++ncol) {
                    if (mask & (1ULL << ncol)) {
                        int idx = p * NUM_PAT + ncol;     /* new state (p, ncol) */
                        nxt[idx] = nxt[idx] + val;
                    }
                }
            }
        }

        /* swap current and next */
        for (int i = 0; i < STATES; ++i) cur[i] = nxt[i];
    }

    /* after all columns added, check the last column (pattern 'p')
       which has only a left neighbour ('pp').                                    */
    seq_t total = 0;
    for (int pp = 0; pp <= SENTINEL; ++pp) {
        for (int p = 0; p < NUM_PAT; ++p) {
            seq_t val = cur[pp * NUM_PAT + p];
            if (val == 0) continue;
            if (_valid_last_column(pp, p))
                total = total + val;
        }
    }

    /* sequence counts arrays identical up to complement,
       therefore divide by 2. */
    return total / 2;
}
