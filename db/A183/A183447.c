/*
 * A183447  Number of nX6 binary arrays with every 1 having exactly two
 *          king-move neighbors equal to 1.
 * https://oeis.org/A183447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A183447
 * Sequence: Number of n×6 binary arrays in which every 1 has exactly
 * two king‑move neighbours equal to 1.
 *
 * Algorithm: dynamic programming over rows.  Each row is a 6‑bit value
 * (0–63).  For every ordered pair (prev,row) we store the number of
 * ways to reach that state.  Transitions to the next row are pre–computed
 * from the predicate valid_row(prev,row,next).  The DP is O(n·6⁴), but
 * with 6 fixed the transition tables are tiny and the algorithm runs
 * in a few milliseconds for n≈10⁴.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* constants */
#define ROWS 64U                    /* 2⁶ possible rows */
#define STATE (ROWS*ROWS)           /* number of (prev,row) pairs */

static inline bool valid_row(uint8_t a, uint8_t b, uint8_t c)
/* returns true iff every 1 in row b has exactly two king‑move neighbours
 * among the three rows a (above), b (current), c (below).  0 outside the
 * array are treated as 0. */
{
    for (int j = 0; j < 6; ++j) {
        if (!(b & (1U << j))) continue;          /* skip 0 cells */
        int cnt = 0;
        /* horizontal neighbours */
        if (j > 0)   cnt += (b >> (j-1)) & 1U;
        if (j < 5)   cnt += (b >> (j+1)) & 1U;
        /* vertical neighbours */
        if (a & (1U << j)) ++cnt;                 /* above   */
        if (c & (1U << j)) ++cnt;                 /* below   */
        /* diagonal neighbours */
        if (j > 0) { cnt += (a >> (j-1)) & 1U;
                     cnt += (c >> (j-1)) & 1U; }
        if (j < 5) { cnt += (a >> (j+1)) & 1U;
                     cnt += (c >> (j+1)) & 1U; }
        if (cnt != 2) return false;
    }
    return true;
}

/* pre‑computed transition tables */
static bool precomputation_done = false;
static uint8_t trans_next[ROWS][ROWS][ROWS];   /* trans_next[a][b][k] = next row */
static uint8_t trans_count[ROWS][ROWS];        /* number of valid next rows */
static bool final_valid[ROWS][ROWS];           /* validity of last row */

/* build the tables once */
static void precompute(void)
{
    if (precomputation_done) return;
    for (uint8_t a = 0; a < ROWS; ++a) {
        for (uint8_t b = 0; b < ROWS; ++b) {
            uint8_t cnt = 0;
            for (uint8_t c = 0; c < ROWS; ++c) {
                if (valid_row(a, b, c))
                    trans_next[a][b][cnt++] = c;
            }
            trans_count[a][b] = cnt;
        }
    }
    for (uint8_t a = 0; a < ROWS; ++a)
        for (uint8_t b = 0; b < ROWS; ++b)
            final_valid[a][b] = valid_row(a, b, 0);
    precomputation_done = true;
}

/* required function --------------------------------------------------- */
seq_t a(idx_t n)
{
    precompute();

    if (n == 0) return (seq_t)0;              /* defined for safety */

    /* n == 1 */
    if (n == 1) {
        seq_t res = 0;
        for (uint8_t r = 0; r < ROWS; ++r)
            if (valid_row(0, r, 0))
                ++res;
        return res;
    }

    /* n == 2 */
    if (n == 2) {
        seq_t res = 0;
        for (uint8_t r0 = 0; r0 < ROWS; ++r0)
            for (uint8_t r1 = 0; r1 < ROWS; ++r1)
                if (valid_row(0, r0, r1) && valid_row(r0, r1, 0))
                    ++res;
        return res;
    }

    /* n >= 3 : DP over states (prev,row) */
    seq_t *dp      = (seq_t *)calloc(STATE, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(STATE, sizeof(seq_t));
    if (!dp || !dp_next) {
        free(dp); free(dp_next);
        return (seq_t)0;
    }

    /* initial state: first two rows */
    for (uint8_t r0 = 0; r0 < ROWS; ++r0)
        for (uint8_t r1 = 0; r1 < ROWS; ++r1)
            if (valid_row(0, r0, r1))
                ++dp[ROWS * r0 + r1];

    /* process remaining rows */
    for (idx_t step = 0; step < n - 2; ++step) {
        /* clear dp_next */
        for (size_t i = 0; i < STATE; ++i) dp_next[i] = 0;

        for (uint8_t p = 0; p < ROWS; ++p)
            for (uint8_t c = 0; c < ROWS; ++c) {
                seq_t val = dp[ROWS * p + c];
                if (!val) continue;
                uint8_t *nexts = trans_next[p][c];
                uint8_t  cnt   = trans_count[p][c];
                for (uint8_t k = 0; k < cnt; ++k) {
                    uint8_t r = nexts[k];
                    dp_next[ROWS * c + r] += val;
                }
            }

        /* swap tables */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    /* sum over states that satisfy the last row */
    seq_t result = 0;
    for (uint8_t p = 0; p < ROWS; ++p)
        for (uint8_t b = 0; b < ROWS; ++b)
            if (final_valid[p][b])
                result += dp[ROWS * p + b];

    free(dp);
    free(dp_next);
    return result;
}
