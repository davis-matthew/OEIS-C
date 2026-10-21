/*
 * A202771  Number of (n+2) X 4 binary arrays avoiding patterns 000 and 011 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 28, u64 n <= 67, u128 n <= 146,
 *            big any n
 * Helpers:   valid_transition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------
   A202771 – Number of (n+2)×4 binary arrays avoiding
   patterns 000 and 011 in rows, columns and NW‑SE diagonals.
   Computed by dynamic programming on admissible 4‑bit rows.
   ------------------------------------------------------------- */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/valid_transition.h"

/* 9 admissible rows (4‑bit patterns with no horizontal 000 or 011)   */
static int R = 0;                    /* number of admissible rows */
static unsigned char rows[16];       /* rows[k] = 4‑bit pattern, valid if k<R */

/* state = pair of last two rows: 0 <= state < R*R   */
static int state_row1[81];           /* first row index of each state */
static int state_row2[81];           /* second row index */

/* adjacency: from a state to next-state indices */
static int adj[81][9];
static int adj_len[81];

/* DP buffers */
static seq_t *dp_cur  = NULL;        /* counts for current number of rows */
static seq_t *dp_next = NULL;        /* temporary for next number of rows */
static int cur_len = 0;              /* current processed number of rows (>=2) */

/* memoised a(n) values (n>=1) */
static seq_t *a_vals   = NULL;
static size_t a_vals_size = 0;

/* -------------------------------------------------------------
   Check that a 4‑bit row has no forbidden horizontal patterns.
   ------------------------------------------------------------- */
static bool row_valid(unsigned char r)
{
    for (int c = 0; c <= 1; ++c) {
        int b0 = (r >> c) & 1;
        int b1 = (r >> (c+1)) & 1;
        int b2 = (r >> (c+2)) & 1;
        if ((b0==0 && b1==0 && b2==0) || (b0==0 && b1==1 && b2==1))
            return false;
    }
    return true;
}

/* -------------------------------------------------------------
   Initialise tables, DP buffers, and the first DP state.
   ------------------------------------------------------------- */
static void init_tables(void)
{
    /* admissible rows */
    for (int i = 0; i < 16; ++i)
        if (row_valid((unsigned char)i)) {
            rows[R++] = (unsigned char)i;
        }

    /* state pairs and adjacency lists */
    int idx = 0;
    for (int i = 0; i < R; ++i)           /* first row of the pair */
        for (int j = 0; j < R; ++j) {     /* second row of the pair */
            state_row1[idx] = i;
            state_row2[idx] = j;
            int cnt = 0;
            for (int l = 0; l < R; ++l) { /* candidate third row */
                if (valid_transition(rows[i], rows[j], rows[l])) {
                    adj[idx][cnt++] = j * R + l;  /* destination state (j,l) */
                }
            }
            adj_len[idx] = cnt;
            ++idx;
        }

    /* allocate DP buffers */
    const int S = R * R;  /* 81 */
    dp_cur  = (seq_t *)malloc(S * sizeof(seq_t));
    dp_next = (seq_t *)malloc(S * sizeof(seq_t));
    memset(dp_cur, 0, S * sizeof(seq_t));
    memset(dp_next, 0, S * sizeof(seq_t));

    /* initial counts for length 2: all admissible pairs are possible */
    for (int s = 0; s < S; ++s) dp_cur[s] = 1;
    cur_len = 2;  /* processed rows = 2 */
}

/* -------------------------------------------------------------
   Extend memoised results up to length m = n+2.
   ------------------------------------------------------------- */
static void ensure_length(int m)
{
    static bool started = false;
    if (!started) {
        init_tables();
        started = true;
    }

    const int S = R * R;
    while (cur_len < m) {
        /* reset dp_next for the new length */
        memset(dp_next, 0, S * sizeof(seq_t));

        /* transitions */
        for (int s = 0; s < S; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;
            int len = adj_len[s];
            for (int k = 0; k < len; ++k) {
                int t = adj[s][k];          /* destination state index */
                dp_next[t] += val;
            }
        }

        /* swap buffers: dp_next becomes new dp_cur */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        ++cur_len;  /* now cur_len equals the new number of rows (m) */

        /* sum all counts to obtain a(n) for n = cur_len-2 */
        seq_t sum = 0;
        for (int s = 0; s < S; ++s) sum += dp_cur[s];
        int n = cur_len - 2;  /* 1‑based index for a(n) */

        /* grow memo table if necessary */
        if ((size_t)n + 1 > a_vals_size) {
            size_t new_size = (size_t)n + 1;
            seq_t *tmp2 = (seq_t *)realloc(a_vals, new_size * sizeof(seq_t));
            if (!tmp2) return;  /* allocation failed – abort silently */
            a_vals = tmp2;
            a_vals_size = new_size;
        }
        a_vals[n] = sum;
    }
}

/* -------------------------------------------------------------
   Public function required by the problem statement.
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* undefined below offset */

    int m = (int)(n + 2);                    /* total rows in the array */
    ensure_length(m);
    return a_vals[(size_t)n];
}
