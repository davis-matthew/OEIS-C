/*
 * A207446  Number of 6Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = number of 6×n binary arrays that contain no horizontal
 *            0 0 1 or 0 1 0 * (in any row) and no vertical 0 0 1 or 1 0 0 (in
 *            any column). * This is computed by a transfer‑matrix dynamic
 *            program over all * admissible 6‑bit columns. The state is a pair
 *            of consecutive columns
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* helper: true if column c has no vertical 001 or 100 */
static inline bool vertical_ok(uint8_t c)
{
    for (int r = 0; r <= 3; ++r) {
        int b0 = (c >> r)     & 1;
        int b1 = (c >> (r+1)) & 1;
        int b2 = (c >> (r+2)) & 1;
        if ((b0==0 && b1==0 && b2==1) || (b0==1 && b1==0 && b2==0))
            return false;
    }
    return true;
}

/* helper: true if columns c1,c2,c3 do not create
 * 0 0 1 or 0 1 0 in any row */
static inline bool horizontal_ok(uint8_t c1, uint8_t c2, uint8_t c3)
{
    for (int r = 0; r < 6; ++r) {
        int b0 = (c1 >> r) & 1;
        int b1 = (c2 >> r) & 1;
        int b2 = (c3 >> r) & 1;
        if ((b0==0 && b1==0 && b2==1) || (b0==0 && b1==1 && b2==0))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static data, built once */
    static bool   initialized = false;
    static uint8_t allowedList[64];
    static int    allowedCount = 0;
    static int    stateCount   = 0;
    static int    *stateC1 = NULL; /* length stateCount */
    static int    *stateC2 = NULL;
    static int    (*state_id)[64] = NULL; /* 64×64 table */

    if (!initialized) {
        /* compute allowed columns */
        for (int c = 0; c < 64; ++c) {
            if (vertical_ok((uint8_t)c)) {
                allowedList[allowedCount++] = (uint8_t)c;
            }
        }
        /* build state mapping: pair (c1,c2) -> id */
        state_id = (int (*)[64])calloc(64 * 64, sizeof(int));
        for (int i = 0; i < 64 * 64; ++i) state_id[0][i] = -1;
        for (int i = 0; i < 64; ++i) for (int j = 0; j < 64; ++j)
            state_id[i][j] = -1; /* init */
        for (int idx = 0; idx < allowedCount; ++idx) {
            int c1 = allowedList[idx];
            for (int jdx = 0; jdx < allowedCount; ++jdx) {
                int c2 = allowedList[jdx];
                state_id[c1][c2] = stateCount++;
            }
        }
        /* allocate per‑state arrays */
        stateC1 = (int *)malloc(stateCount * sizeof(int));
        stateC2 = (int *)malloc(stateCount * sizeof(int));
        for (int idx = 0; idx < allowedCount; ++idx) {
            int c1 = allowedList[idx];
            for (int jdx = 0; jdx < allowedCount; ++jdx) {
                int c2 = allowedList[jdx];
                int id  = state_id[c1][c2];
                stateC1[id] = c1;
                stateC2[id] = c2;
            }
        }
        initialized = true;
    }

    if (n == 0) /* undefined in OEIS but return 0 safely */
        return (seq_t)0;

    /* n == 1 */
    if (n == 1) {
        return (seq_t)allowedCount;
    }

    /* n == 2 */
    if (n == 2) {
        return ((seq_t)allowedCount) * ((seq_t)allowedCount);
    }

    /* n >= 3 : DP over pairs of columns */
    seq_t *dp_prev = (seq_t *)calloc(stateCount, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(stateCount, sizeof(seq_t));

    /* initialize for n==2 */
    for (int s = 0; s < stateCount; ++s)
        dp_prev[s] = 1; /* one way for each starting pair */

    for (idx_t step = 3; step <= n; ++step) {
        /* zero dp_next */
        for (int i = 0; i < stateCount; ++i) dp_next[i] = 0;

        for (int s = 0; s < stateCount; ++s) {
            seq_t val = dp_prev[s];
            if (!val) continue;
            int c1 = stateC1[s];
            int c2 = stateC2[s];

            for (int tdx = 0; tdx < allowedCount; ++tdx) {
                uint8_t c3 = allowedList[tdx];
                if (!horizontal_ok((uint8_t)c1, (uint8_t)c2, c3))
                    continue;
                int tid = state_id[c2][c3];
                dp_next[tid] += val;
            }
        }
        /* swap */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
    }

    /* sum all states to obtain total count */
    seq_t total = 0;
    for (int s = 0; s < stateCount; ++s)
        total += dp_prev[s];

    free(dp_prev);
    free(dp_next);

    return total;
}
