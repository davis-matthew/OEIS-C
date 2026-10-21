/*
 * A208430  Number of nX4 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to any knight-move neighbor
 *          (colorings ignoring permutations of colors).
 * https://oeis.org/A208430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS A208430: Number of n×4 arrays colored with {0,1,2} 
   where colors are introduced in row‑major order (canonical labeling) 
   and no two knight‑move neighbors have the same color. 
   The computation is performed by dynamic programming over rows, 
   tracking the two previous rows and the current maximum color used. 
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Forward declarations: types provided by the build. */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* Constants for encoding rows and states. */
#define NUM_CUTS 81               /* 3^4 possible rows (0..80) */
#define NO_ROW   NUM_CUTS          /* sentinel for "no previous row" */
#define STATE_BASE (NUM_CUTS+1)    /* include sentinel */
#define NUM_STATES (STATE_BASE*STATE_BASE*3)   /* last row, two‑before row, max color */

/* --- Global precomputation data --- */
static uint8_t decode[NUM_CUTS][4];
static unsigned char canonCount[3];
typedef struct { uint8_t id; uint8_t newm; } canon_entry_t;
static canon_entry_t canon[3][NUM_CUTS];

static unsigned char transCount[NUM_STATES];
static uint16_t transNext[NUM_STATES][81];   /* all possible successors (packed) */

static bool initialized = false;

/* Precompute all tables used by the DP. */
static void precompute(void)
{
    /* Decode each row id into its 4 column colors. */
    for (int id = 0; id < NUM_CUTS; ++id) {
        int x = id;
        for (int c = 0; c < 4; ++c) {
            decode[id][c] = x % 3;
            x /= 3;
        }
    }

    /* Canonical lists for each starting maximum color. */
    for (int id = 0; id < NUM_CUTS; ++id) {
        uint8_t col[4];
        for (int c = 0; c < 4; ++c) col[c] = decode[id][c];
        for (int m0 = 0; m0 <= 2; ++m0) {
            int m = m0;
            bool ok = true;
            for (int i = 0; i < 4 && ok; ++i) {
                uint8_t colc = col[i];
                if (colc > m + 1) { ok = false; break; }
                if (colc == m + 1) {
                    if (m == 2) { ok = false; break; }
                    ++m;
                }
            }
            if (ok) {
                canon[m0][canonCount[m0]++] = (canon_entry_t){ .id = (uint8_t)id, .newm = (uint8_t)m };
            }
        }
    }

    /* Transition table for every state (prev1, prev2, currMax). */
    for (int p1 = 0; p1 <= NO_ROW; ++p1) {
        for (int p2 = 0; p2 <= NO_ROW; ++p2) {
            for (int km = 0; km <= 2; ++km) {
                int stateIdx = ((p1 * STATE_BASE) + p2) * 3 + km;
                unsigned char cnt = 0;
                /* For each candidate next row respecting canonical order. */
                for (int ci = 0; ci < canonCount[km]; ++ci) {
                    uint8_t candID = canon[km][ci].id;
                    uint8_t newm   = canon[km][ci].newm;
                    bool ok = true;
                    /* Check knight constraints with previous row. */
                    if (p1 != NO_ROW) {
                        for (int c = 0; c < 4 && ok; ++c) {
                            uint8_t newVal = decode[candID][c];
                            if (c >= 2 && newVal == decode[p1][c-2]) ok = false;
                            else if (c <= 1 && newVal == decode[p1][c+2]) ok = false;
                        }
                    }
                    /* Check knight constraints with two‑rows‑above row. */
                    if (ok && p2 != NO_ROW) {
                        for (int c = 0; c < 4 && ok; ++c) {
                            uint8_t newVal = decode[candID][c];
                            if (c >= 1 && newVal == decode[p2][c-1]) ok = false;
                            else if (c <= 2 && newVal == decode[p2][c+1]) ok = false;
                        }
                    }
                    if (ok) {
                        int newState = ((candID * STATE_BASE) + p1) * 3 + newm;
                        transNext[stateIdx][cnt++] = (uint16_t)newState;
                    }
                }
                transCount[stateIdx] = cnt;
            }
        }
    }
    initialized = true;
}

/* --- Sequence function --- */
seq_t a(idx_t n)
{
    if (!initialized) precompute();

    /* Allocate DP arrays only once. */
    static seq_t *dp = NULL;
    static seq_t *next = NULL;
    static int  *stateActive = NULL;
    static int  *stateNext   = NULL;
    static bool *seen        = NULL;

    if (!dp) {
        dp = (seq_t *)calloc(NUM_STATES, sizeof(seq_t));
        next = (seq_t *)calloc(NUM_STATES, sizeof(seq_t));
        stateActive = (int *)malloc(NUM_STATES * sizeof(int));
        stateNext = (int *)malloc(NUM_STATES * sizeof(int));
        seen = (bool *)malloc(NUM_STATES * sizeof(bool));
    }

    /* Reset dp array. */
    for (int i = 0; i < NUM_STATES; ++i) dp[i] = 0;

    /* --- Initialize row 1 states --- */
    int activeCnt = 0;
    for (int ci = 0; ci < canonCount[0]; ++ci) {
        /* The first cell of the board must be color 0. */
        if (decode[canon[0][ci].id][0] != 0) continue;
        int id    = canon[0][ci].id;
        int newm  = canon[0][ci].newm;
        int state = ((id * STATE_BASE) + NO_ROW) * 3 + newm;
        dp[state] += 1;
        stateActive[activeCnt++] = state;
    }

    if (n == 1) {
        seq_t total = 0;
        for (int i = 0; i < activeCnt; ++i) total += dp[stateActive[i]];
        return total;
    }

    /* --- Iteratively add rows 2..n --- */
    for (idx_t row = 2; row <= n; ++row) {
        /* Clear next array and tracking structures. */
        for (int i = 0; i < NUM_STATES; ++i) next[i] = 0;
        for (int i = 0; i < NUM_STATES; ++i) seen[i] = false;
        int nextCnt = 0;

        /* Scan active states from previous row. */
        for (int si = 0; si < activeCnt; ++si) {
            int state = stateActive[si];
            seq_t ways = dp[state];
            unsigned char tcnt = transCount[state];
            for (int tj = 0; tj < tcnt; ++tj) {
                int nstate = transNext[state][tj];
                if (!seen[nstate]) {
                    seen[nstate] = true;
                    stateNext[nextCnt++] = nstate;
                }
                next[nstate] += ways;
            }
        }

        /* Prepare for next iteration. */
        seq_t *tmp = dp; dp = next; next = tmp;
        int *tmpi = stateActive; stateActive = stateNext; stateNext = tmpi;
        activeCnt = nextCnt;
    }

    /* Sum all ways after the last row. */
    seq_t result = 0;
    for (int i = 0; i < activeCnt; ++i) result += dp[stateActive[i]];
    return result;
}
