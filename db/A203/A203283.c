/*
 * A203283  Number of nX6 binary arrays with every element neighboring
 *          horizontally or vertically both a 0 and a 1.
 * https://oeis.org/A203283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   counts the number of n×6 binary arrays such that every cell has
 *            at least one 0‑neighbour and at least one 1‑neighbour among its
 *            horizontal or vertical neighbours. The computation is performed
 *            with dynamic programming over the rows, treating a state as the
 *            pair of the previous two row patterns
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required external types */
#include <stdint.h>
#include <string.h>
#include <stddef.h>

#define NUM_PAT 64
#define NUM_SP 65              /* 64 normal patterns + sentinel */
#define TOTAL_STATES (NUM_SP * NUM_PAT)
#define MAX_DEG 64

static unsigned char missingH[NUM_PAT][6];
static unsigned char need0_tbl[NUM_PAT][NUM_SP];
static unsigned char need1_tbl[NUM_PAT][NUM_SP];
static uint16_t next_idx[TOTAL_STATES][MAX_DEG];
static unsigned char next_deg[TOTAL_STATES];
static int initialized = 0;

static void init(void)
{
    int p, c;
    /* compute horizontal missing mask for each pattern & column */
    for (p = 0; p < NUM_PAT; ++p) {
        for (c = 0; c < 6; ++c) {
            unsigned char m = 0;
            if (c > 0) {
                unsigned char bv = (p >> (c - 1)) & 1;
                m |= (bv == 0) ? 1 : 2;
            }
            if (c < 5) {
                unsigned char bv = (p >> (c + 1)) & 1;
                m |= (bv == 0) ? 1 : 2;
            }
            missingH[p][c] = (~m) & 3u;  /* 0: none; 1: need 0; 2: need 1 */
        }
    }

    /* compute needed masks for a row given the row above */
    for (int prev = 0; prev < NUM_PAT; ++prev) {
        for (int prevPrev = 0; prevPrev < NUM_SP; ++prevPrev) {
            unsigned char need0 = 0, need1 = 0;
            for (c = 0; c < 6; ++c) {
                unsigned char missing = missingH[prev][c];
                if (prevPrev != NUM_SP - 1) { /* not sentinel */
                    unsigned char above = (prevPrev >> c) & 1;
                    if ((missing & 1) && above == 0) missing ^= 1;
                    if ((missing & 2) && above == 1) missing ^= 2;
                }
                if (missing & 1) need0 |= (1u << c);
                if (missing & 2) need1 |= (1u << c);
            }
            need0_tbl[prev][prevPrev] = need0;
            need1_tbl[prev][prevPrev] = need1;
        }
    }

    /* precompute adjacency lists for DP */
    for (int prevPrev = 0; prevPrev < NUM_SP; ++prevPrev) {
        for (int prev = 0; prev < NUM_PAT; ++prev) {
            int sid = prevPrev * NUM_PAT + prev;
            unsigned char need0 = need0_tbl[prev][prevPrev];
            unsigned char need1 = need1_tbl[prev][prevPrev];
            unsigned char deg = 0;
            for (int curr = 0; curr < NUM_PAT; ++curr) {
                if (((curr & need0) == 0) &&
                    (((~curr) & 0x3F) & need1) == 0) {
                    next_idx[sid][deg++] = prev * NUM_PAT + curr;
                }
            }
            next_deg[sid] = deg;
        }
    }

    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 1) return 0;          /* first term: 0 */

    static seq_t dp_cur[TOTAL_STATES];
    static seq_t dp_next[TOTAL_STATES];
    memset(dp_cur, 0, sizeof(dp_cur));

    const int sent = NUM_SP - 1;           /* sentinel index = 64 */

    /* initial states: first row patterns */
    for (int p = 0; p < NUM_PAT; ++p) {
        dp_cur[sent * NUM_PAT + p] = 1;
    }

    /* process rows 2..n */
    idx_t row;
    for (row = 2; row <= n; ++row) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int sid = 0; sid < TOTAL_STATES; ++sid) {
            seq_t val = dp_cur[sid];
            if (val == 0) continue;
            unsigned char deg = next_deg[sid];
            uint16_t *nbr = next_idx[sid];
            for (unsigned char k = 0; k < deg; ++k) {
                dp_next[nbr[k]] += val;
            }
        }
        /* swap dp arrays */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    /* final summation: last row must have no pending requirement */
    seq_t result = 0;
    for (int sid = 0; sid < TOTAL_STATES; ++sid) {
        seq_t ways = dp_cur[sid];
        if (ways == 0) continue;
        int prevPrev = sid / NUM_PAT;
        int prev = sid % NUM_PAT;
        if (need0_tbl[prev][prevPrev] == 0 &&
            need1_tbl[prev][prevPrev] == 0) {
            result += ways;
        }
    }
    return result;
}
