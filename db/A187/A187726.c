/*
 * A187726  Number of (n+1)X7 binary arrays with no 2X2 subblock commuting
 *          with any of its horizontal and vertical 2X2 subblock neighbors.
 * https://oeis.org/A187726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   blocks_commute, get_block
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A187726: number of (n+1)×7 binary arrays with no 2×2 subblock
   commuting with any of its horizontal and vertical 2×2 subblock neighbors.
   The algorithm uses dynamic programming over pairs of consecutive rows.
   For each pair of rows (a,b) the 6 horizontal 2×2 blocks are checked
   for non‑commutativity.  When a third row c is appended, the pair
   (b,c) must also be horizontally valid and every vertical pair of
   neighboring 2×2 blocks (formed by rows a,b and b,c) must not commute.
   The transition graph is pre‑computed once; then the number of ways to
   reach each state after k steps is updated iteratively.  This runs in
   O(n·E) time with E ≈ 2·10⁴, well below 2 s for n ≤ 10 000. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/blocks_commute.h"
#include "../utils/get_block.h"

#define WIDTH 7
#define ROWS 128
#define STATES (ROWS * ROWS)
#define MAX_TRANS 128     /* maximum number of next rows per state (≤128) */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* transition tables                                                     */
static bool   initialized = false;
static uint8_t trans[STATES][MAX_TRANS];   /* next rows for each state         */
static unsigned char nextCnt[STATES];      /* number of transitions for a state */
static uint32_t initStates[STATES];        /* all horizontally valid pairs    */
static size_t   initStateCnt = 0;

static void init_transition(void)
{
    uint32_t a, b, c, idx;
    for (a = 0; a < ROWS; ++a)
    {
        for (b = 0; b < ROWS; ++b)
        {
            /* check horizontal validity of pair (a,b) */
            bool horiz_ok = true;
            for (uint8_t j = 0; j <= WIDTH - 3; ++j)   /* j = 0 … 4 */
            {
                uint8_t B1 = get_block((uint8_t)a, (uint8_t)b, j);
                uint8_t B2 = get_block((uint8_t)a, (uint8_t)b, j + 1);
                if (blocks_commute(B1, B2))
                {
                    horiz_ok = false;
                    break;
                }
            }
            if (!horiz_ok) continue;

            idx = a * ROWS + b;
            initStates[initStateCnt++] = idx;
            unsigned char cnt = 0;

            for (c = 0; c < ROWS; ++c)
            {
                /* check horizontal validity of (b,c) */
                bool horiz_ok2 = true;
                for (uint8_t j = 0; j <= WIDTH - 3; ++j)
                {
                    uint8_t B1 = get_block((uint8_t)b, (uint8_t)c, j);
                    uint8_t B2 = get_block((uint8_t)b, (uint8_t)c, j + 1);
                    if (blocks_commute(B1, B2))
                    {
                        horiz_ok2 = false;
                        break;
                    }
                }
                if (!horiz_ok2) continue;

                /* vertical constraint between (a,b) and (b,c) */
                bool vert_ok = true;
                for (uint8_t j = 0; j <= WIDTH - 2; ++j)   /* j = 0 … 5 */
                {
                    uint8_t B1 = get_block((uint8_t)a, (uint8_t)b, j);
                    uint8_t B2 = get_block((uint8_t)b, (uint8_t)c, j);
                    if (blocks_commute(B1, B2))
                    {
                        vert_ok = false;
                        break;
                    }
                }
                if (!vert_ok) continue;

                trans[idx][cnt++] = (uint8_t)c;
            }
            nextCnt[idx] = cnt;
        }
    }
    initialized = true;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if (!initialized) init_transition();

    static seq_t dpCur[STATES];
    static seq_t dpNext[STATES];
    static uint32_t actIdx[STATES];
    static uint32_t nextActIdx[STATES];
    static bool     visited[STATES];
    size_t actCnt = 0, nextCntAct = 0;

    /* initialise current DP with all horizontally valid pairs */
    memset(dpCur, 0, sizeof(dpCur));
    for (size_t i = 0; i < initStateCnt; ++i)
    {
        uint32_t idx = initStates[i];
        dpCur[idx] = 1;
        actIdx[actCnt++] = idx;
    }

    /* iterate over remaining rows (total n+1 rows => n steps) */
    for (idx_t step = 1; step <= n - 1; ++step)
    {
        memset(dpNext, 0, sizeof(dpNext));
        memset(visited, 0, sizeof(visited));
        nextCntAct = 0;

        for (size_t s = 0; s < actCnt; ++s)
        {
            uint32_t idx = actIdx[s];
            seq_t val = dpCur[idx];
            if (val == 0) continue;

            uint8_t b = (uint8_t)(idx & 127);
            unsigned char cCnt = nextCnt[idx];
            for (unsigned char k = 0; k < cCnt; ++k)
            {
                uint8_t c = trans[idx][k];
                uint32_t nIdx = (b * ROWS) + c;  /* pair (b,c) */
                dpNext[nIdx] += val;
                if (!visited[nIdx])
                {
                    visited[nIdx] = true;
                    nextActIdx[nextCntAct++] = nIdx;
                }
            }
        }

        /* prepare for next iteration */
        memcpy(dpCur, dpNext, sizeof(dpCur));
        memcpy(actIdx, nextActIdx, nextCntAct * sizeof(uint32_t));
        actCnt = nextCntAct;
    }

    /* sum all counts in the final state set */
    seq_t result = 0;
    for (size_t i = 0; i < actCnt; ++i)
        result += dpCur[actIdx[i]];
    return result;
}
