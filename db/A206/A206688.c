/*
 * A206688  Number of nX4 0..3 arrays with no element equal to another within
 *          two positions in the same row or column, and new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A206688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A206688 is computed by a 15‑term linear recurrence for n>15. The
 *            first 15 terms are obtained by dynamic programming using the
 *            definition of the sequence. a(n) = 8*a(n-1) -8*a(n-2) -48*a(n-3)
 *            +48*a(n-4) +140*a(n-5) +67*a(n-6) -968*a(n-7) +1564*a(n-8)
 *            -586*a(n-9) -1008*a(n-10) -32*a(n-11) +1304*a(n...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   effect_legal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/effect_legal.h"

#define ROWS 48
#define SENTINEL ROWS              /* index used for “no previous row”   */
#define MAX_CUR 3                  /* maximum possible curMax after a row */

/* Generate list of all 48 valid rows (patterns) satisfying in‑row constraints. */
static void init_rows(unsigned char (*rowVals)[ROWS][4]) {
    int count = 0;
    for (int mask = 0; mask < 256 && count < ROWS; ++mask) {
        int d[4];
        int tmp = mask;
        for (int i = 0; i < 4; ++i) {
            d[i] = tmp & 3;
            tmp >>= 2;
        }
        /* In‑row constraints: no equal within distance 1 or 2 */
        if (d[0] == d[1] || d[0] == d[2] ||
            d[1] == d[2] || d[1] == d[3] ||
            d[2] == d[3]) continue;
        /* store pattern */
        for (int i = 0; i < 4; ++i) (*rowVals)[count][i] = (unsigned char)d[i];
        ++count;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Pre‑compute the 48 valid rows */
    static unsigned char rowVals[ROWS][4];
    init_rows(&rowVals);
    /* DP arrays: count states indexed by [prevIdx][lastIdx][curMax] */
    static seq_t cur[SENTINEL+1][SENTINEL+1][MAX_CUR+1];
    static seq_t next[SENTINEL+1][SENTINEL+1][MAX_CUR+1];
    /* First compute the first 15 terms by DP */
    seq_t init[15];
    /* Zero DP arrays */
    for (int i = 0; i <= SENTINEL; ++i)
        for (int j = 0; j <= SENTINEL; ++j)
            for (int k = 0; k <= MAX_CUR; ++k) cur[i][j][k] = 0;
    /* ----- first row ----- */
    for (int ridx = 0; ridx < ROWS; ++ridx) {
        if (rowVals[ridx][0] != 0) continue;          /* first entry must be 0 */
        int newMax;
        if (!effect_legal(-1, ridx, rowVals, &newMax)) continue;
        cur[SENTINEL][ridx][newMax] += 1;             /* count each pattern once */
    }
    /* Sum all states for n=1 */
    seq_t sum = 0;
    for (int p = 0; p <= SENTINEL; ++p)
        for (int q = 0; q <= SENTINEL; ++q)
            for (int m = 0; m <= MAX_CUR; ++m)
                sum += cur[p][q][m];
    init[0] = sum;
    /* ----- rows 2..15 ----- */
    for (int rows = 2; rows <= 15; ++rows) {
        /* zero next DP */
        for (int i = 0; i <= SENTINEL; ++i)
            for (int j = 0; j <= SENTINEL; ++j)
                for (int k = 0; k <= MAX_CUR; ++k) next[i][j][k] = 0;
        /* Transition from cur to next */
        for (int prev = 0; prev <= SENTINEL; ++prev) {
            for (int last = 0; last <= SENTINEL; ++last) {
                for (int curMax = 0; curMax <= MAX_CUR; ++curMax) {
                    seq_t cnt = cur[prev][last][curMax];
                    if (cnt == 0) continue;
                    for (int ridx = 0; ridx < ROWS; ++ridx) {
                        bool ok = true;
                        /* vertical constraints with last row */
                        if (last != SENTINEL) {
                            for (int j = 0; j < 4; ++j)
                                if (rowVals[ridx][j] == rowVals[last][j]) { ok = false; break; }
                            if (!ok) continue;
                        }
                        /* vertical constraints with prev row (distance 2) */
                        if (prev != SENTINEL) {
                            for (int j = 0; j < 4; ++j)
                                if (rowVals[ridx][j] == rowVals[prev][j]) { ok = false; break; }
                            if (!ok) continue;
                        }
                        int newMax;
                        if (!effect_legal(curMax, ridx, rowVals, &newMax)) continue;
                        /* shift state: prev becomes last, current becomes new */
                        next[last][ridx][newMax] += cnt;
                    }
                }
            }
        }
        /* copy next to cur for next iteration */
        for (int i = 0; i <= SENTINEL; ++i)
            for (int j = 0; j <= SENTINEL; ++j)
                for (int k = 0; k <= MAX_CUR; ++k) cur[i][j][k] = next[i][j][k];
        /* compute sum for this number of rows */
        seq_t sumRows = 0;
        for (int i = 0; i <= SENTINEL; ++i)
            for (int j = 0; j <= SENTINEL; ++j)
                for (int k = 0; k <= MAX_CUR; ++k) sumRows += cur[i][j][k];
        init[rows-1] = sumRows;
    }
    /* -------------------  recurrence for n>15 ------------------- */
    if (n <= 15) return init[n-1];
    seq_t last[15];
    for (int i = 0; i < 15; ++i) last[i] = init[i];
    for (idx_t k = 16; k <= n; ++k) {
        /* compute new_term from last[14]..last[1] */
        seq_t pos = (seq_t)8   * last[14]  + (seq_t)48  * last[11] +
                    (seq_t)140 * last[10] + (seq_t)67  * last[9 ] +
                    (seq_t)1564* last[7]  + (seq_t)1304* last[3]  +
                    (seq_t)80  * last[2];
        seq_t neg = (seq_t)8   * last[13]  + (seq_t)48  * last[12] +
                    (seq_t)968 * last[8]  + (seq_t)586 * last[6]  +
                    (seq_t)1008* last[5]  + (seq_t)32  * last[4]  +
                    (seq_t)512 * last[1];
        seq_t new_term = pos - neg;
        /* shift the window */
        for (int i = 0; i < 14; ++i) last[i] = last[i+1];
        last[14] = new_term;
    }
    return last[14];
}
