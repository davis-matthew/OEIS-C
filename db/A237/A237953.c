/*
 * A237953  Number of (n+1) X (2+1) 0..3 arrays with the maximum plus the
 *          upper median plus the lower median plus the minimum of every 2 X 2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A237953
 *
 *  The sequence counts (n+1)×3 arrays with entries in {0,1,2,3}
 *  such that, for every 2×2 subblock, the sum of its four entries
 *  differs from the sums of its horizontal and vertical neighbours
 *  by exactly one.
 *
 *  We solve this by dynamic programming over pairs of consecutive rows.
 *  Each row is encoded as a 6‑bit number (3 cells × 2 bits).
 *  There are 64 possible rows and 4096 possible adjacent‑row states.
 *  Pre‑compute all admissible transitions – the result for a given n
 *  is obtained by iterating n‑1 transitions from the initial state set.
 *
 *  No closed form is known; the implementation simply follows the
 *  definition.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define ROWS   64U            /* 3 cells × 2 bits = 6 bits ⇒ 0..63 */
#define STATES (ROWS * ROWS)  /* 4096 */

/* Tables for row encodings and pre‑computed sums */
static uint8_t rowVals[ROWS][3];
static uint8_t s0[ROWS][ROWS];      /* sum of left 2×2 subblock */
static uint8_t s1[ROWS][ROWS];      /* sum of right 2×2 subblock */
static bool   horiz_ok[ROWS][ROWS]; /* a pair (p,q) is horizontally valid */

/* Transition lists for each state */
static uint16_t nextStates[STATES][64];
static uint8_t  nextCount[STATES];

static seq_t baseCounts[STATES];   /* dp counts for n = 1 */

/* DP buffers – reused for each call */
static seq_t dp_curr[STATES];
static seq_t dp_next[STATES];

static bool initialized = false;

/* Build all tables once */
static void init(void)
{
    int p, q, r;

    /* Row encodings */
    for (p = 0; p < ROWS; ++p) {
        rowVals[p][0] =  p & 3;
        rowVals[p][1] = (p >> 2) & 3;
        rowVals[p][2] = (p >> 4) & 3;
    }

    /* Pre‑compute block sums and horizontal validity */
    for (p = 0; p < ROWS; ++p) {
        for (q = 0; q < ROWS; ++q) {
            uint8_t a = rowVals[p][0];
            uint8_t b = rowVals[p][1];
            uint8_t c = rowVals[p][2];
            uint8_t d = rowVals[q][0];
            uint8_t e = rowVals[q][1];
            uint8_t f = rowVals[q][2];

            s0[p][q] = a + b + d + e;
            s1[p][q] = b + c + e + f;
            horiz_ok[p][q] =
                ((s0[p][q] > s1[p][q]) ? s0[p][q] - s1[p][q]
                                       : s1[p][q] - s0[p][q]) == 1;
        }
    }

    /* Build transition lists */
    for (p = 0; p < ROWS; ++p) {
        for (q = 0; q < ROWS; ++q) {
            int pi = p * ROWS + q;
            if (!horiz_ok[p][q]) { nextCount[pi] = 0; continue; }

            uint8_t cnt = 0;
            for (r = 0; r < ROWS; ++r) {
                if (!horiz_ok[q][r]) continue;

                /* Vertical constraints between (p,q) and (q,r) */
                uint8_t diff0 = (s0[p][q] > s0[q][r]) ? s0[p][q] - s0[q][r]
                                                     : s0[q][r] - s0[p][q];
                if (diff0 != 1) continue;
                uint8_t diff1 = (s1[p][q] > s1[q][r]) ? s1[p][q] - s1[q][r]
                                                     : s1[q][r] - s1[p][q];
                if (diff1 != 1) continue;

                nextStates[pi][cnt++] = (uint16_t)(q * ROWS + r);
            }
            nextCount[pi] = cnt;
        }
    }

    /* Base counts for n = 1 (two rows). */
    memset(baseCounts, 0, sizeof(baseCounts));
    for (p = 0; p < ROWS; ++p) {
        for (q = 0; q < ROWS; ++q) {
            if (horiz_ok[p][q])
                baseCounts[p * ROWS + q] = 1;
        }
    }

    initialized = true;
}

/* Main function required by the framework */
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    /* For n = 1 simply sum the base counts. */
    if (n == 1) {
        seq_t total = 0;
        for (int i = 0; i < STATES; ++i)
            total += baseCounts[i];
        return total;
    }

    /* Start DP with counts for n = 1. */
    memcpy(dp_curr, baseCounts, sizeof(dp_curr));

    /* Perform n‑1 transitions. */
    for (idx_t step = 2; step <= n; ++step) {
        /* Clear dp_next */
        memset(dp_next, 0, sizeof(dp_next));

        /* Transition step */
        for (int i = 0; i < STATES; ++i) {
            seq_t val = dp_curr[i];
            if (!val) continue;
            uint8_t allow = nextCount[i];
            for (uint8_t k = 0; k < allow; ++k) {
                uint16_t j = nextStates[i][k];
                dp_next[j] += val;
            }
        }

        /* Swap dp_curr and dp_next */
        seq_t *tmp = dp_curr;
        memcpy(dp_curr, dp_next, sizeof(dp_next));
        memcpy(dp_next, tmp, sizeof(tmp[0]) * STATES); /* clear old dp_next but not needed */
    }

    /* Sum all end states */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i)
        total += dp_curr[i];

    return total;
}
