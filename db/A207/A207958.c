/*
 * A207958  Number of nX6 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of n×6 binary arrays avoiding the
 * horizontally forbidden patterns 000 and 101, and vertically
 * forbidden patterns 001 and 100 (OEIS A207958).  The sequence
 * is computed by dynamic programming over the 19 row masks that
 * satisfy the horizontal constraints.  For every pair of consecutive
 * rows, all admissible following rows are pre‑computed and the DP
 * counts all possible extensions.
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define WIDTH   6
#define MASKS   (1 << WIDTH)          /* 64 */
#define NUM_POS 19                    /* number of masks that avoid 000 and 101 */

/* state for a single row mask that satisfies horizontal constraints */
static int  validMask[NUM_POS];            /* the actual 6‑bit masks */
static int  maskIndex[MASKS];              /* index of a mask in validMask, -1 if invalid */

/* transitions for adding a third row r after rows p,q   */
static int   nextMask[NUM_POS][NUM_POS][NUM_POS];   /* nextMask[p][q][k] = index r */
static uint8_t nextCount[NUM_POS][NUM_POS];        /* number of valid r for each pair */

/* ------------------------------------------------------------------
 * Initialise the lookup tables once.
 * ------------------------------------------------------------------ */
static void
init_masks(void)
{
    static bool done = false;
    if (done)
        return;
    done = true;

    /* mark all masks as invalid */
    for (int i = 0; i < MASKS; ++i)
        maskIndex[i] = -1;

    /* determine horizontally admissible masks */
    int cnt = 0;
    for (int m = 0; m < MASKS; ++m) {
        bool ok = true;
        for (int j = 0; j <= WIDTH - 3; ++j) {
            int trip = (m >> j) & 7;        /* three consecutive bits */
            if (trip == 0x0 || trip == 0b101) { /* 000 or 101 */
                ok = false;
                break;
            }
        }
        if (ok) {
            maskIndex[m] = cnt;
            validMask[cnt++] = m;
        }
    }
    assert(cnt == NUM_POS);    /* we must find exactly 19 masks */

    /* transitions for adding a new row */
    for (int p = 0; p < NUM_POS; ++p) {
        for (int q = 0; q < NUM_POS; ++q) {
            int pc = 0;
            for (int r = 0; r < NUM_POS; ++r) {
                int pm = validMask[p];
                int qm = validMask[q];
                int rm = validMask[r];
                bool ok = true;
                for (int col = 0; col < WIDTH; ++col) {
                    int a = (pm >> col) & 1;
                    int b = (qm >> col) & 1;
                    int c = (rm >> col) & 1;
                    /* forbidden vertical patterns: 001 or 100 */
                    if (a == 0 && b == 0 && c == 1) { ok = false; break; }
                    if (a == 1 && b == 0 && c == 0) { ok = false; break; }
                }
                if (ok) {
                    nextMask[p][q][pc++] = r;
                }
            }
            nextCount[p][q] = (uint8_t)pc;
        }
    }
}

/* ------------------------------------------------------------------
 * Main accessor required by the framework.
 * ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)0;          /* undefined but harmless */
    if (n == 1) return (seq_t)19;         /* single row, only horizontal restriction */

    init_masks();

    /* DP states: count of ways for the last two rows having indices p,q */
    seq_t dp[NUM_POS][NUM_POS];
    seq_t ndp[NUM_POS][NUM_POS];

    /* initialise dp for n = 2 (no vertical constraints yet) */
    for (int i = 0; i < NUM_POS; ++i)
        for (int j = 0; j < NUM_POS; ++j)
            dp[i][j] = (seq_t)1;

    /* if we only need a(2) stop here */
    if (n == 2) {
        seq_t total = 0;
        for (int i = 0; i < NUM_POS; ++i)
            for (int j = 0; j < NUM_POS; ++j)
                total += dp[i][j];
        return total;
    }

    /* iterate from the 3rd row up to row n */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero ndp */
        for (int i = 0; i < NUM_POS; ++i)
            for (int j = 0; j < NUM_POS; ++j)
                ndp[i][j] = (seq_t)0;

        for (int p = 0; p < NUM_POS; ++p) {
            for (int q = 0; q < NUM_POS; ++q) {
                seq_t ways = dp[p][q];
                if (ways == (seq_t)0) continue;
                uint8_t cnt = nextCount[p][q];
                for (uint8_t t = 0; t < cnt; ++t) {
                    int r = nextMask[p][q][t];
                    ndp[q][r] += ways;
                }
            }
        }
        /* copy ndp back to dp for next iteration */
        for (int i = 0; i < NUM_POS; ++i)
            for (int j = 0; j < NUM_POS; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final tally */
    seq_t total = 0;
    for (int p = 0; p < NUM_POS; ++p)
        for (int q = 0; q < NUM_POS; ++q)
            total += dp[p][q];
    return total;
}
