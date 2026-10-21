/*
 * A206784  Number of nX7 0..1 arrays avoiding 0 0 1 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A206784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206784: */
/* Number of n×7 binary arrays avoiding the horizontal pattern 0 0 1 and the vertical pattern 0 1 1. */
/* Computed by dynamic programming over pairs of consecutive rows. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ROWS   54                  /* number of 7‑bit rows without the pattern 001 */
#define STATES (ROWS * ROWS)       /* state index (prev, curr) */

/* Row masks – one byte per row pattern */
static uint8_t rowMask[ROWS];

/* Transition information: for each state (prev, curr) */
static uint8_t  transCnt[STATES];           /* number of allowed next states */
static uint16_t transNext[STATES][ROWS];    /* list of those next states */

/* Initialization flag */
static int initialized = 0;

/* ------------------------------------------------------------------ */
/* initialize the static tables once                                    */
/* ------------------------------------------------------------------ */
static void init(void)
{
    if (initialized) return;
    initialized = 1;

    /* enumerate all 7‑bit rows and keep only those avoiding 001 */
    int k = 0;
    for (int m = 0; m < 128; ++m) {
        int ok = 1;
        for (int i = 0; i <= 4; ++i) {
            int b0 = (m >> i)     & 1;
            int b1 = (m >> (i + 1)) & 1;
            int b2 = (m >> (i + 2)) & 1;
            if (b0 == 0 && b1 == 0 && b2 == 1) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            rowMask[k++] = (uint8_t)m;
        }
    }

    /* build transition tables for each pair of rows (prev, curr) */
    for (int p = 0; p < ROWS; ++p) {
        uint8_t pMask = rowMask[p];
        for (int c = 0; c < ROWS; ++c) {
            uint8_t cMask = rowMask[c];
            int state = p * ROWS + c;
            uint8_t cnt = 0;

            /* columns where we forbid a 1 in the next row: (prev bit 0 & curr bit 1) */
            uint8_t forbid = (~pMask & 0x7F) & cMask;

            for (int n = 0; n < ROWS; ++n) {
                if ((rowMask[n] & forbid) == 0) {
                    int nextState = c * ROWS + n;
                    transNext[state][cnt++] = (uint16_t)nextState;
                }
            }
            transCnt[state] = cnt;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init();                                   /* ensure tables are ready */

    if (n == 0) return (seq_t)0;             /* undefined, return 0 */

    if (n == 1) return (seq_t)ROWS;          /* single row */
    if (n == 2) return (seq_t)ROWS * (seq_t)ROWS;  /* two rows, no vertical constraint */

    /* DP arrays over states: dp_cur holds counts for current pair (prev, curr) */
    static seq_t dp_cur[STATES];
    static seq_t dp_next[STATES];
    seq_t *cur = dp_cur;
    seq_t *next = dp_next;

    /* initialise for the first two rows: all pairs are allowed */
    for (int i = 0; i < STATES; ++i) cur[i] = (seq_t)1;

    /* iterate rows 3 to n */
    for (idx_t row = 3; row <= n; ++row) {
        memset(next, 0, sizeof(dp_next));
        for (int s = 0; s < STATES; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            uint8_t cnt = transCnt[s];
            uint16_t *nxt = transNext[s];
            for (uint8_t k = 0; k < cnt; ++k) {
                next[nxt[k]] += val;
            }
        }
        /* swap cur and next for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum all final states to obtain the total number of arrays */
    seq_t result = (seq_t)0;
    for (int s = 0; s < STATES; ++s) result += cur[s];
    return result;
}
