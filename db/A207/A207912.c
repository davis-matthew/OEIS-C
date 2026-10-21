/*
 * A207912  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 0 1 1 vertically.
 * https://oeis.org/A207912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   horizontal_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207912: Number of 7×n binary matrices avoiding the horizontal
   patterns 000 and 010 and the vertical patterns 000 and 011.  The
   sequence is computed by a transfer‑matrix method on pairs of
   admissible columns.  The implementation builds the set of all valid
   7‑bit columns, precomputes the adjacency of pairs of columns, and
   then performs a DP that is O(n·C²) where C is the number of valid
   columns (C≈34 for the 7×N case).  The code works for all n≥1 and
   stores intermediate results in static arrays so that repeated calls
   in arbitrary order are inexpensive. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/horizontal_ok.h"

/*------------------------------------------------------------------------------*/
/*  Helper functions to test admissible columns                                 */
/*------------------------------------------------------------------------------*/
static bool vertical_ok(uint8_t v)
{
    for (int r = 0; r <= 4; ++r) {
        int b0 = (v >> r) & 1;
        int b1 = (v >> (r+1)) & 1;
        int b2 = (v >> (r+2)) & 1;
        if ((b0==0 && b1==0 && b2==0) || (b0==0 && b1==1 && b2==1))
            return false;
    }
    return true;
}

/*------------------------------------------------------------------------------*/
/*  Static data for the transfer matrix                                        */
/*------------------------------------------------------------------------------*/
static bool initialized = false;
static int      num_cols;      /* number of admissible columns */
static uint8_t *col_bits;      /* array of 7‑bit values */

static int      num_states;    /* num_cols * num_cols */
static int    **trans;         /* trans[s][k]  - next state indices */
static int    *trans_count;    /* number of transitions from state s */

/*------------------------------------------------------------------------------*/
/*  Build the column set and the transition table                              */
/*------------------------------------------------------------------------------*/
static void init(void)
{
    if (initialized)
        return;

    /* first gather all admissible 7‑bit columns */
    num_cols = 0;
    uint8_t tmp[128];
    for (int v = 0; v < 128; ++v) {
        if (vertical_ok((uint8_t)v)) {
            tmp[num_cols++] = (uint8_t)v;
        }
    }
    col_bits = (uint8_t*)malloc(num_cols * sizeof(uint8_t));
    memcpy(col_bits, tmp, num_cols * sizeof(uint8_t));

    num_states = num_cols * num_cols;
    trans       = (int**)malloc(num_states * sizeof(int*));
    trans_count = (int*)  malloc(num_states * sizeof(int));

    for (int s = 0; s < num_states; ++s) {
        int prev = s / num_cols;
        int cur  = s % num_cols;
        uint8_t pv = col_bits[prev];
        uint8_t cv = col_bits[cur];
        /* we do not know a priori how many transitions this state will have,
           but it is at most num_cols, so allocate that many slots first.   */
        int *list = (int*)malloc(num_cols * sizeof(int));
        int cnt = 0;
        for (int nxt = 0; nxt < num_cols; ++nxt) {
            uint8_t nv = col_bits[nxt];
            if (horizontal_ok(pv, cv, nv)) {
                list[cnt++] = cur * num_cols + nxt;
            }
        }
        /* shrink list to actual size */
        if (cnt < num_cols) {
            int *shrinked = (int*)realloc(list, cnt * sizeof(int));
            if (shrinked)
                list = shrinked;
        }
        trans[s]       = list;
        trans_count[s] = cnt;
    }

    initialized = true;
}

/*------------------------------------------------------------------------------*/
/*  Main sequence routine                                                       */
/*------------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    init();

    /* special handling for the first two lengths – no horizontal restriction */
    if (n == 1)
        return (seq_t)num_cols;
    if (n == 2)
        return (seq_t)num_cols * (seq_t)num_cols;

    /* DP over the state space (pairs of columns).  dp_prev[s] holds the
       number of ways to reach state s after the current length.               */
    seq_t *dp_prev = (seq_t*)calloc(num_states, sizeof(seq_t));
    seq_t *dp_cur  = (seq_t*)calloc(num_states, sizeof(seq_t));
    if (!dp_prev || !dp_cur) { /* memory allocation failure */
        /* In the unlikely event of OOM we return 0; the caller can retry.   */
        return (seq_t)0;
    }

    /* length = 2 : all pairs are admissible */
    for (int s = 0; s < num_states; ++s)
        dp_prev[s] = (seq_t)1;

    for (idx_t len = 3; len <= n; ++len) {
        for (int i = 0; i < num_states; ++i)
            dp_cur[i] = (seq_t)0;

        for (int s = 0; s < num_states; ++s) {
            if (dp_prev[s] == 0)
                continue;
            seq_t ways = dp_prev[s];
            int cnt = trans_count[s];
            int *list = trans[s];
            for (int t = 0; t < cnt; ++t) {
                int ns = list[t];
                dp_cur[ns] += ways;
            }
        }
        /* swap dp_prev and dp_cur for next iteration */
        seq_t *tmp  = dp_prev;
        dp_prev     = dp_cur;
        dp_cur      = tmp;
    }

    /* sum over all final states to obtain total number of matrices */
    seq_t total = (seq_t)0;
    for (int s = 0; s < num_states; ++s)
        total += dp_prev[s];

    free(dp_prev);
    free(dp_cur);

    return total;
}
