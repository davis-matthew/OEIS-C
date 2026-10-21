/*
 * A223920  Number of 3Xn 0..2 arrays with rows and antidiagonals unimodal and
 *          columns nondecreasing.
 * https://oeis.org/A223920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 22, u64 n <= 160, u128 n <= 210,
 *            big any n
 * Helpers:   decode_phase, encode_phase
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: DP counting 3×n 0..2 arrays with rows and
   antidiagonals unimodal and columns nondecreasing (OEIS A223920). */

#include <stdbool.h>
#include "../utils/decode_phase.h"
#include "../utils/encode_phase.h"

/* Number of nondecreasing triples over {0,1,2}.  Row 1 ≤ row 2 ≤ row 3. */
enum { COL_TYPES = 10, PHASE_COUNT = 8, OLD_R3_COUNT = 3, STATE_COUNT = COL_TYPES * PHASE_COUNT * OLD_R3_COUNT };

/* Precomputed values of the 10 nondecreasing columns. */
static const unsigned char col_v1[COL_TYPES] = {0,0,0,0,0,0,1,1,1,2};
static const unsigned char col_v2[COL_TYPES] = {0,0,0,1,1,2,1,1,2,2};
static const unsigned char col_v3[COL_TYPES] = {0,1,2,1,2,2,1,2,2,2};

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;      /* offset 1 – not used but harmless */

    seq_t dp_cur[STATE_COUNT];
    seq_t dp_next[STATE_COUNT];

    /* Initialise for column 1: all phases increasing, prev‑2 row 3 = 0. */
    for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 0;
    for (int col = 0; col < COL_TYPES; ++col) {
        int state = (col * PHASE_COUNT + 0) * OLD_R3_COUNT + 0;
        dp_cur[state] = dp_cur[state] + 1;   /* one array per column type */
    }

    /* Process columns 2 … n. */
    for (idx_t colIdx = 2; colIdx <= n; ++colIdx) {
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (int cur = 0; cur < STATE_COUNT; ++cur) {
            seq_t curCnt = dp_cur[cur];
            if (curCnt == 0) continue;

            /* Decode current state: colPrev, phase, prev2_r3. */
            int tmp = cur;
            unsigned char prev2_r3 = tmp % OLD_R3_COUNT; tmp /= OLD_R3_COUNT;
            unsigned char phaseIdx = tmp % PHASE_COUNT; tmp /= PHASE_COUNT;
            int colPrev = tmp;

            unsigned char p1, p2, p3;
            decode_phase(phaseIdx, &p1, &p2, &p3);

            unsigned char v1_prev = col_v1[colPrev];
            unsigned char v2_prev = col_v2[colPrev];
            unsigned char v3_prev = col_v3[colPrev];

            /* Enumerate all possible next column types. */
            for (int colNext = 0; colNext < COL_TYPES; ++colNext) {
                unsigned char v1_cur = col_v1[colNext];
                unsigned char v2_cur = col_v2[colNext];
                unsigned char v3_cur = col_v3[colNext];

                unsigned char np1, np2, np3;
                bool ok = true;

                /* Row 1 transition. */
                if (p1 == 0) {                       /* still increasing */
                    if (v1_cur < v1_prev) np1 = 1;    /* start decreasing */
                    else                    np1 = 0;
                } else {                              /* already decreasing */
                    if (v1_cur > v1_prev) { ok = false; continue; }
                    np1 = 1;
                }

                /* Row 2 transition. */
                if (p2 == 0) {
                    if (v2_cur < v2_prev) np2 = 1;
                    else                  np2 = 0;
                } else {
                    if (v2_cur > v2_prev) { ok = false; continue; }
                    np2 = 1;
                }

                /* Row 3 transition. */
                if (p3 == 0) {
                    if (v3_cur < v3_prev) np3 = 1;
                    else                  np3 = 0;
                } else {
                    if (v3_cur > v3_prev) { ok = false; continue; }
                    np3 = 1;
                }

                if (!ok) continue;

                /* Antidiagonal unimodality check for columns ≥ 3. */
                if (colIdx >= 3) {
                    /* anti‑diagonal elements: v1_cur (row 1, column j),
                                               v2_prev (row 2, column j‑1),
                                               prev2_r3 (row 3, column j‑2). */
                    if (v2_prev < v1_cur && v2_prev < prev2_r3) continue; /* valley */
                }

                /* New state: column j+1 details. */
                unsigned char newPhaseIdx = encode_phase(np1, np2, np3);
                /* For the next state, prev‑2 row 3 becomes the previous column's
                   row 3 (row 3 of column j). */
                unsigned char newPrev2_r3 = v3_prev;   /* row 3 of column j   */
                int newState = (colNext * PHASE_COUNT + newPhaseIdx) * OLD_R3_COUNT + newPrev2_r3;

                dp_next[newState] = dp_next[newState] + curCnt;
            }
        }
        /* Move to next column. */
        for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum over all ending states to obtain a(n). */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i) total = total + dp_cur[i];

    return total;
}
