/*
 * A208163  Number of nX7 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A208163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic programming for A208163 – # of n×7 binary arrays avoiding
   horizontal patterns 000, 101 and vertical patterns 001, 010. */

#include <stdlib.h>
#include <stdbool.h>

/* Number of allowed 7‑bit rows. */
#define ROWS 28
#define STATES (ROWS * ROWS)

/* Tables of allowed rows and transitions (one‑time init). */
static unsigned char rows[ROWS];
static unsigned char zeroMask[STATES];
static unsigned char oneMask[STATES];
static unsigned char nextCount[STATES];
static unsigned short nextIdx[STATES][ROWS];

/* DP buffers. */
static seq_t *curDp = NULL;
static seq_t *nextDp = NULL;
static bool initialized = false;

/* Initialise row lists and transition tables. */
static void init_tables(void) {
    if (initialized) return;

    /* Enumerate all 7‑bit rows avoiding 000 and 101 horizontally. */
    int rc = 0;
    for (int r = 0; r < 128; ++r) {
        bool ok = true;
        for (int j = 0; j <= 4; ++j) {
            int val = (r >> j) & 7;          /* 3‑bit window */
            if (val == 0 || val == 5) {      /* 000 or 101 */
                ok = false;
                break;
            }
        }
        if (ok) rows[rc++] = (unsigned char)r;
    }

    /* Pre‑compute transitions from a pair (prev, cur) to next. */
    for (int aIdx = 0; aIdx < ROWS; ++aIdx) {
        int a = rows[aIdx];
        for (int bIdx = 0; bIdx < ROWS; ++bIdx) {
            int b = rows[bIdx];
            int state = aIdx * ROWS + bIdx;
            int zm = (~a & ~b) & 0x7F;          /* columns where a=0,b=0 */
            int om = (~a &  b) & 0x7F;          /* columns where a=0,b=1 */
            zeroMask[state] = (unsigned char)zm;
            oneMask [state] = (unsigned char)om;

            int cnt = 0;
            for (int cIdx = 0; cIdx < ROWS; ++cIdx) {
                int c = rows[cIdx];
                if (((c & zm) == 0) && ((c & om) == om)) {
                    nextIdx[state][cnt++] = (unsigned short)(bIdx * ROWS + cIdx);
                }
            }
            nextCount[state] = (unsigned char)cnt;
        }
    }

    /* Allocate DP arrays. */
    curDp  = (seq_t*)malloc(STATES * sizeof(seq_t));
    nextDp = (seq_t*)malloc(STATES * sizeof(seq_t));

    initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;         /* undefined for n=0 */

    init_tables();

    if (n == 1) return (seq_t)28;         /* single row */

    /* Each state represents a pair of rows; initialize count = 1. */
    for (int i = 0; i < STATES; ++i) curDp[i] = (seq_t)1;

    idx_t len = 2;
    while (len < n) {
        /* zero next DP buffer */
        for (int i = 0; i < STATES; ++i) nextDp[i] = (seq_t)0;

        /* transition */
        for (int s = 0; s < STATES; ++s) {
            seq_t cnt = curDp[s];
            if (cnt == (seq_t)0) continue;
            int tcnt = nextCount[s];
            for (int t = 0; t < tcnt; ++t) {
                nextDp[nextIdx[s][t]] += cnt;
            }
        }

        /* swap */
        seq_t *tmp = curDp;
        curDp = nextDp;
        nextDp = tmp;
        ++len;
    }

    /* sum all final states */
    seq_t res = (seq_t)0;
    for (int i = 0; i < STATES; ++i) res += curDp[i];
    return res;
}
