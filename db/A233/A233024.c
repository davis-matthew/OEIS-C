/*
 * A233024  Number of nX6 0..3 arrays with no element x(i,j) adjacent to value
 *          3-x(i,j) horizontally, vertically, diagonally or antidiagonally,
 *          and top left element zero.
 * https://oeis.org/A233024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A233024 – number of n×6 arrays with entries 0..3, top‑left fixed to 0,
 *  and no adjacent pair (including the 8 directions) summing to 3.
 *  The code uses dynamic programming over all legal 6‑tuples for a single row
 *  and a precomputed compatibility table between consecutive rows. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------- */
/*  Precomputation of row states and compatibility                 */
/* --------------------------------------------------------------- */
static bool initialized = false;
static size_t stateCnt;          /* number of legal 6‑tuples */
static uint8_t rows[4096][6];    /* rows[i][p] – value in position p of state i  */
static size_t *neighCnt;         /* number of neighbours for each state */
static size_t *offset;           /* offset of the neighbour list for a state */
static size_t *neigh;            /* flattened neighbour lists */

/* --------------------------------------------------------------- */
static void precompute(void)
{
    /* Enumerate all 6‑tuples of 0..3 that satisfy the horizontal restriction
     *        – no two consecutive entries sum to 3.
     */
    size_t map[4096];
    for (size_t i = 0; i < 4096; ++i) map[i] = (size_t)-1;

    stateCnt = 0;
    for (size_t code = 0; code < 4096; ++code) {
        uint8_t val[6];
        size_t tmp = code;
        for (int pos = 5; pos >= 0; --pos) {
            val[pos] = (uint8_t)(tmp & 3u);
            tmp >>= 2;
        }
        bool ok = true;
        for (int pos = 0; pos < 5 && ok; ++pos) {
            if ((uint16_t)val[pos] + val[pos + 1] == 3u) ok = false;
        }
        if (!ok) continue;

        map[code] = stateCnt;
        for (int pos = 0; pos < 6; ++pos) rows[stateCnt][pos] = val[pos];
        ++stateCnt;
    }

    /* Compute compatibility between rows */
    neighCnt = (size_t *)malloc(stateCnt * sizeof(size_t));
    for (size_t i = 0; i < stateCnt; ++i) neighCnt[i] = 0;

    for (size_t i = 0; i < stateCnt; ++i) {
        const uint8_t *ri = rows[i];
        for (size_t j = 0; j < stateCnt; ++j) {
            const uint8_t *rj = rows[j];
            bool compat = true;
            for (int p = 0; p < 6 && compat; ++p) {
                if (ri[p] + rj[p] == 3u)                compat = false;
                else if (p > 0 && ri[p-1] + rj[p] == 3u) compat = false;
                else if (p < 5 && ri[p+1] + rj[p] == 3u) compat = false;
            }
            if (compat) ++neighCnt[i];
        }
    }

    /* Allocate flattened neighbour list */
    offset = (size_t *)malloc(stateCnt * sizeof(size_t));
    size_t total = 0;
    for (size_t i = 0; i < stateCnt; ++i) {
        offset[i] = total;
        total += neighCnt[i];
    }

    neigh = (size_t *)malloc(total * sizeof(size_t));
    size_t *cur = (size_t *)calloc(stateCnt, sizeof(size_t));

    for (size_t i = 0; i < stateCnt; ++i) {
        const uint8_t *ri = rows[i];
        for (size_t j = 0; j < stateCnt; ++j) {
            const uint8_t *rj = rows[j];
            bool compat = true;
            for (int p = 0; p < 6 && compat; ++p) {
                if (ri[p] + rj[p] == 3u)                compat = false;
                else if (p > 0 && ri[p-1] + rj[p] == 3u) compat = false;
                else if (p < 5 && ri[p+1] + rj[p] == 3u) compat = false;
            }
            if (compat) {
                size_t pos = offset[i] + cur[i];
                neigh[pos] = j;
                ++cur[i];
            }
        }
    }
    free(cur);
    initialized = true;
}

/* --------------------------------------------------------------- */
/*  Main sequence function                                         */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) precompute();

    /* Dynamic programming arrays */
    seq_t *curr = (seq_t *)calloc(stateCnt, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(stateCnt, sizeof(seq_t));

    /* First row – must start with 0 */
    for (size_t i = 0; i < stateCnt; ++i) {
        if (rows[i][0] == 0u) curr[i] = (seq_t)1;
    }

    /* Process subsequent rows */
    for (idx_t r = 1; r < n; ++r) {
        /* zero next */
        for (size_t i = 0; i < stateCnt; ++i) next[i] = (seq_t)0;

        for (size_t i = 0; i < stateCnt; ++i) {
            seq_t val = curr[i];
            if (val == (seq_t)0) continue;
            size_t start = offset[i];
            size_t end   = start + neighCnt[i];
            for (size_t k = start; k < end; ++k) {
                size_t j = neigh[k];
                next[j] += val;
            }
        }
        /* swap arrays */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* Sum counts of all final rows */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < stateCnt; ++i) result += curr[i];

    free(curr);
    free(next);
    return result;
}
