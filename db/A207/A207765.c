/*
 * A207765  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP over the last two columns. For every 5×n binary array we keep
 *            the last two columns and extend the array only when horizontal
 *            triple‑patterns (000 or 101) are avoided. Vertical restrictions
 *            are applied to each single column. The DP state space is the set
 *            of all pairs of admissible columns, so t...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Pre‑computed data shared between calls (generated once).           */
/* ------------------------------------------------------------------ */
static bool            initialized = false;
static uint8_t         column_patterns[32];
static int             col_cnt = 0;
static int             adjacency[1024][32];
static size_t          adjacency_size[1024];
static size_t          state_count = 0;
static seq_t          *dp_cur  = NULL;
static seq_t          *dp_next = NULL;
static seq_t          *total   = NULL;
static size_t          total_cap = 0;
static idx_t           max_n_comp = 0;

/* ------------------------------------------------------------------ */
/*  Helper: check whether a single 5‑bit column respects vertical     */
/*  forbidden patterns 011 and 101.                                    */
/* ------------------------------------------------------------------ */
static bool vertical_ok(uint8_t pat)
{
    for (int r = 0; r <= 2; ++r) {
        int a = (pat >> r) & 1;
        int b = (pat >> (r + 1)) & 1;
        int c = (pat >> (r + 2)) & 1;
        if ((a == 0 && b == 1 && c == 1) || (a == 1 && b == 0 && c == 1))
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Initialise all data structures on first call.                      */
/* ------------------------------------------------------------------ */
static void init(void)
{
    /* collect all admissible column patterns (vertical check)      */
    col_cnt = 0;
    for (int pat = 0; pat < 32; ++pat) {
        if (vertical_ok((uint8_t)pat))
            column_patterns[col_cnt++] = (uint8_t)pat;
    }

    /* number of DP states : pair of columns                    */
    state_count = (size_t)col_cnt * (size_t)col_cnt;

    /* build transition lists: from (i,j) to all allowed k         */
    for (int i = 0; i < col_cnt; ++i) {
        for (int j = 0; j < col_cnt; ++j) {
            size_t s = (size_t)i * col_cnt + j;
            size_t sz = 0;
            for (int k = 0; k < col_cnt; ++k) {
                bool ok = true;
                for (int r = 0; r < 5; ++r) {
                    int b1 = (column_patterns[i] >> r) & 1;
                    int b2 = (column_patterns[j] >> r) & 1;
                    int b3 = (column_patterns[k] >> r) & 1;
                    if ((b1 == 0 && b2 == 0 && b3 == 0) ||
                        (b1 == 1 && b2 == 0 && b3 == 1)) {
                        ok = false;
                        break;
                    }
                }
                if (ok)
                    adjacency[s][sz++] = k;
            }
            adjacency_size[s] = sz;
        }
    }

    /* allocate DP arrays (state_count <= 1024)                     */
    dp_cur  = (seq_t *)malloc(state_count * sizeof(seq_t));
    dp_next = (seq_t *)malloc(state_count * sizeof(seq_t));

    /* initial DP for length 2: each pair of admissible columns is 1*/
    for (size_t i = 0; i < state_count; ++i) {
        dp_cur[i]  = (seq_t)1;
        dp_next[i] = (seq_t)0;
    }

    /* allocate total array for n = 0,1,2                                    */
    total_cap = 3;
    total = (seq_t *)malloc(total_cap * sizeof(seq_t));
    total[0] = (seq_t)0;
    total[1] = (seq_t)col_cnt;                           /* a(1)    */
    total[2] = (seq_t)col_cnt * (seq_t)col_cnt;           /* a(2)    */
    max_n_comp = 2;

    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Public entry point : compute a(n).                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 0)
        return (seq_t)0;

    /* first time: build tables */
    if (!initialized)
        init();

    /* If value already computed, return it immediately */
    if ((idx_t)max_n_comp >= n)
        return total[n];

    /* Ensure total array large enough */
    if ((size_t)n + 1 > total_cap) {
        total_cap = (size_t)n + 1;
        total = (seq_t *)realloc(total, total_cap * sizeof(seq_t));
    }

    /* Compute DP from max_n_comp + 1 up to n                           */
    for (size_t len = (size_t)max_n_comp + 1; len <= (size_t)n; ++len) {
        if (len <= 2) {
            /* values already stored in total */
            continue;
        }

        /* reset next DP layer */
        for (size_t i = 0; i < state_count; ++i)
            dp_next[i] = (seq_t)0;

        /* transition from previous DP layer */
        for (size_t s = 0; s < state_count; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0)
                continue;
            size_t sz = adjacency_size[s];
            for (size_t k = 0; k < sz; ++k) {
                int next_col = adjacency[s][k];
                size_t new_state = ((s % col_cnt) * col_cnt) + next_col;
                dp_next[new_state] += val;
            }
        }

        /* sum all DP states to obtain total for current width */
        seq_t cumulative = (seq_t)0;
        for (size_t i = 0; i < state_count; ++i)
            cumulative += dp_next[i];
        total[len] = cumulative;

        /* swap DP layers for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    max_n_comp = (idx_t)n;
    return total[n];
}
