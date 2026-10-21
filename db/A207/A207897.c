/*
 * A207897  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207897
   Number of 4×n 0–1 arrays avoiding 0 0 0 and 1 0 1 horizontally
   and 0 0 0 and 1 1 1 vertically.

   The implementation uses dynamic programming on the last two
   columns.  There are 2⁴=16 possible column patterns; only 10 of them
   satisfy the vertical restrictions.  A state is a pair of columns
   (c1,c2).  For each length n≥3 we count the ways to reach each state
   by adding a new column that extends a valid previous state and
   satisfies the horizontal rules.  The sum of the counts for all
   states gives a(n).  Results are memoised in a static array so that
   arbitrary call order is supported and subsequent calls are O(1).
*/

#include <stdlib.h>
#include <stdbool.h>

/* constants */
#define COLS     16           /* 4 rows → 2⁴ column patterns */
#define STATES   256          /* 16 × 16 possible pairs of columns */
#define MAX_VALID 16          /* maximum number of vertical‑valid columns */

/* ------------------------------------------------------------------ */
/*                      Pre‑computed tables (initialised once)        */
/* ------------------------------------------------------------------ */
static unsigned char valid_cols[MAX_VALID];
static int valid_count;

/* transition allowed[s][p] == true if state s (c1,c2) can be extended
   with column p to satisfy horizontal rules. */
static bool allowed[STATES][COLS];

/* ------------------------------------------------------------------ */
/*                      DP storage and memoisation                    */
/* ------------------------------------------------------------------ */
static seq_t *dp0 = NULL;            /* current DP array */
static seq_t *dp1 = NULL;            /* next DP array */
static seq_t *dp_curr = NULL;
static seq_t *dp_next = NULL;

static size_t current_len = 0;        /* number of columns represented by dp_curr */
static seq_t *values = NULL;         /* memoised a[1..N] */
static size_t values_size = 0;

/* ------------------------------------------------------------------ */
static void init_tables(void)
{
    /* determine vertical‑valid column patterns */
    valid_count = 0;
    for (int col = 0; col < COLS; ++col) {
        bool ok = true;
        for (int tr = 0; tr < 2 && ok; ++tr) {
            int b0 = (col >> tr) & 1;
            int b1 = (col >> (tr + 1)) & 1;
            int b2 = (col >> (tr + 2)) & 1;
            if (b0 == b1 && b1 == b2) ok = false;
        }
        if (ok) valid_cols[valid_count++] = (unsigned char)col;
    }

    /* precompute transition table */
    for (int s = 0; s < STATES; ++s) {
        int c1 = s >> 4;
        int c2 = s & 0xF;
        for (int p = 0; p < COLS; ++p) {
            bool trans_ok = true;
            for (int r = 0; r < 4; ++r) {
                int a = (c1 >> r) & 1;
                int b = (c2 >> r) & 1;
                int c = (p >> r) & 1;
                if ((a == 0 && b == 0 && c == 0) ||
                    (a == 1 && b == 0 && c == 1)) {
                    trans_ok = false;
                    break;
                }
            }
            allowed[s][p] = trans_ok;
        }
    }

    /* allocate DP arrays */
    dp0 = (seq_t *)malloc(STATES * sizeof(seq_t));
    dp1 = (seq_t *)malloc(STATES * sizeof(seq_t));
    dp_curr = dp0;
    dp_next = dp1;
    for (int i = 0; i < STATES; ++i) dp_curr[i] = dp_next[i] = 0;
}

/* ------------------------------------------------------------------ */
static void ensure_values(size_t need)
{
    if (values_size >= need) return;
    seq_t *newv = (seq_t *)realloc(values, need * sizeof(seq_t));
    if (!newv) abort();  /* allocation failure */
    values = newv;
    if (values_size < need) {
        for (size_t i = values_size; i < need; ++i) values[i] = 0;
    }
    values_size = need;
}

/* ------------------------------------------------------------------ */
static void init_dp_for_len2(void)
{
    for (int i = 0; i < STATES; ++i) dp_curr[i] = 0;
    for (int i = 0; i < valid_count; ++i) {
        int c1 = valid_cols[i];
        for (int j = 0; j < valid_count; ++j) {
            int c2 = valid_cols[j];
            dp_curr[(c1 << 4) | c2] = 1;
        }
    }
}

/* ------------------------------------------------------------------ */
static void compute_a_up_to(size_t n)
{
    static bool tables_done = false;
    if (!tables_done) {
        init_tables();
        tables_done = true;
    }

    if (n == 1) {
        ensure_values(1);
        values[0] = (seq_t)valid_count;
        return;
    }

    if (current_len == 0) {
        /* initialise base values */
        ensure_values(1);
        values[0] = (seq_t)valid_count;                      /* a(1) */
        ensure_values(2);
        values[1] = (seq_t)((seq_t)valid_count * (seq_t)valid_count); /* a(2) */
        init_dp_for_len2();
        current_len = 2;
    }

    if (n > values_size) ensure_values(n);

    while (current_len < n) {
        for (int i = 0; i < STATES; ++i) dp_next[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_curr[s];
            if (val == 0) continue;
            for (int idx = 0; idx < valid_count; ++idx) {
                int p = valid_cols[idx];
                if (!allowed[s][p]) continue;
                int t = ((s & 0xF) << 4) | p;
                dp_next[t] += val;
            }
        }

        seq_t *tmp = dp_curr;
        dp_curr = dp_next;
        dp_next = tmp;

        ++current_len;

        seq_t sum = 0;
        for (int i = 0; i < STATES; ++i) sum += dp_curr[i];
        values[current_len - 1] = sum;
    }
}

/* ------------------------------------------------------------------ */
/*                     Public function a(idx_t n)                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    compute_a_up_to((size_t)n);
    return values[(size_t)n - 1];
}
