/*
 * A234425  Number of (n+1)X(2+1) 0..7 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to 14
 *          and no adjacent elements equal.
 * https://oeis.org/A234425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A234425
    Number of (n+1) x 3 arrays with entries in 0..7, no equal adjacents,
    and each 2x2 subblock having the sum of absolute edge differences equal to 14.
    DP over all 3‑tuples: the answer is the number of length‑(n+1) walks
    in the directed graph whose vertices are the 392 admissible rows
    and whose edges encode the 2x2‑block condition.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- internal data structures ---------- */
typedef struct {
    uint8_t x[3];   /* cell values 0..7   */
    uint8_t h01;    /* |x0 - x1| (1..7)  */
    uint8_t h12;    /* |x1 - x2| (1..7)  */
} row_t;

#define MAX_STATES 392
#define MAX_NEIGH  64   /* generous upper bound for out‑degree */

/* global state, built once */
static row_t      rows[MAX_STATES];
static uint16_t   neigh[MAX_STATES][MAX_NEIGH];
static uint8_t    neighCnt[MAX_STATES];
static size_t    rowCount = 0;

/* ---------- build the graph once ---------- */
static void _build(void)
{
    /* generate all admissible rows */
    size_t idx = 0;
    for (uint8_t a = 0; a < 8; ++a)
        for (uint8_t b = 0; b < 8; ++b)
            if (a != b)
                for (uint8_t c = 0; c < 8; ++c)
                    if (b != c) {
                        rows[idx].x[0] = a;
                        rows[idx].x[1] = b;
                        rows[idx].x[2] = c;
                        rows[idx].h01 = (a > b) ? (a - b) : (b - a);
                        rows[idx].h12 = (b > c) ? (b - c) : (c - b);
                        ++idx;
                    }
    rowCount = idx;  /* should be 392 */

    /* build adjacency lists */
    for (size_t i = 0; i < rowCount; ++i) {
        neighCnt[i] = 0;
        for (size_t j = 0; j < rowCount; ++j) {
            uint8_t d0 = (rows[i].x[0] > rows[j].x[0]) ?
                         (rows[i].x[0] - rows[j].x[0]) :
                         (rows[j].x[0] - rows[i].x[0]);
            if (d0 == 0) continue;
            uint8_t d1 = (rows[i].x[1] > rows[j].x[1]) ?
                         (rows[i].x[1] - rows[j].x[1]) :
                         (rows[j].x[1] - rows[i].x[1]);
            if (d1 == 0) continue;
            uint8_t d2 = (rows[i].x[2] > rows[j].x[2]) ?
                         (rows[i].x[2] - rows[j].x[2]) :
                         (rows[j].x[2] - rows[i].x[2]);
            if (d2 == 0) continue;

            int leftSum  = rows[i].h01 + rows[j].h01 + d0 + d1;
            if (leftSum != 14) continue;
            int rightSum = rows[i].h12 + rows[j].h12 + d1 + d2;
            if (rightSum != 14) continue;

            neigh[i][neighCnt[i]++] = (uint16_t)j;
        }
    }
}

/* ---------- the requested function ---------- */
seq_t a(idx_t n)
{
    static bool built = false;
    if (!built) { _build(); built = true; }

    /* DP arrays: dp_cur holds counts for rows of current length,
       dp_next for next length. */
    seq_t dp_cur[MAX_STATES];
    seq_t dp_next[MAX_STATES];
    seq_t total = 0;
    size_t S = rowCount;          /* 392 */

    /* initialise for length 1 (single row) */
    for (size_t i = 0; i < S; ++i) dp_cur[i] = 1;
    for (size_t i = 0; i < S; ++i) dp_next[i] = 0;

    idx_t steps = n;  /* number of transitions = (n+1)-1 = n */
    for (idx_t step = 0; step < steps; ++step) {
        /* clear next array */
        for (size_t i = 0; i < S; ++i) dp_next[i] = 0;

        /* transition */
        for (size_t i = 0; i < S; ++i) {
            if (dp_cur[i] == 0) continue;
            seq_t val = dp_cur[i];
            uint8_t cnt = neighCnt[i];
            for (uint8_t k = 0; k < cnt; ++k) {
                dp_next[neigh[i][k]] += val;
            }
        }

        /* swap pointers (dp_cur <-> dp_next) */
        for (size_t i = 0; i < S; ++i) {
            dp_cur[i] = dp_next[i];
        }
    }

    /* sum over all ending rows */
    for (size_t i = 0; i < S; ++i) total += dp_cur[i];
    return total;
}
