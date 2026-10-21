/*
 * A202375  Number of (n+2)X7 binary arrays avoiding patterns 001 and 111 in
 *          rows and columns.
 * https://oeis.org/A202375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 47, u128 n <= 120,
 *            big any n
 * Helpers:   row_has_bad_triple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   DP for A202375 – number of (n+2)×7 binary arrays avoiding
    the patterns 001 and 111 in every row and every column.   */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/row_has_bad_triple.h"

/* maximum number of 7‑bit rows (2^7 = 128) */
#define MAXM 128

/* global tables, initialised once */
static uint8_t rows[MAXM];          /* admissible 7‑bit rows */
static int    M = 0;                /* number of admissible rows */
static bool   init_done = false;

/* test whether adding a third row causes a forbidden vertical
   triple in any column */
static inline bool transition_ok(uint8_t a, uint8_t b, uint8_t c)
{
    for (int col = 0; col < 7; ++col) {
        int aa = (a >> col) & 1;
        int bb = (b >> col) & 1;
        int cc = (c >> col) & 1;
        if ((aa == 0 && bb == 0 && cc == 1) || (aa == 1 && bb == 1 && cc == 1))
            return false;
    }
    return true;
}

/* initialise the list of admissible rows */
static void init_tables(void)
{
    if (init_done) return;
    M = 0;
    for (int i = 0; i < (1 << 7); ++i) {
        if (!row_has_bad_triple((uint8_t)i))
            rows[M++] = (uint8_t)i;
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    init_tables();

    /* number of rows = n+2 (offset 1 → 3 rows) */
    idx_t L = n + 2;

    /* DP tables */
    static seq_t dp[2][MAXM * MAXM];
    seq_t *cur  = dp[0];
    seq_t *nxt  = dp[1];

    /* clear DP tables */
    for (size_t t = 0; t < (size_t)M * (size_t)M; ++t) {
        cur[t] = 0;
        nxt[t] = 0;
    }

    /* initialise after two rows: any admissible pair is possible */
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            cur[i * M + j] = 1;

    /* add the remaining rows one by one */
    for (idx_t rowIdx = 2; rowIdx < L; ++rowIdx) {
        /* clear next layer */
        for (size_t t = 0; t < (size_t)M * (size_t)M; ++t)
            nxt[t] = 0;

        /* transition */
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                seq_t val = cur[i * M + j];
                if (!val) continue;
                for (int k = 0; k < M; ++k) {
                    if (transition_ok(rows[i], rows[j], rows[k])) {
                        nxt[j * M + k] += val;
                    }
                }
            }
        }

        /* swap layers */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum all final states */
    seq_t res = 0;
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            res += cur[i * M + j];

    return res;
}
