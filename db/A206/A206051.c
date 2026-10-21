/*
 * A206051  Number of (n+1)X6 0..2 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to one.
 * https://oeis.org/A206051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A206051: Number of (n+1)×6 arrays with entries 0..2 such that
   each 2×2 subblock has exactly one clockwise edge increase.
   The function computes the terms by dynamic programming over rows;
   it runs in O(n · 3^12) time and uses only the allowed operations.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#if defined(__cplusplus)
#  define NEW   new
#  define DEL   delete[]
#else
#  define NEW   (void*)
#  define DEL   free
#endif

/* 3^6 possible rows */
#define ROWS 729

/* Arrays used for dynamic programming */
static seq_t dp_prev[ROWS];
static seq_t dp_curr[ROWS];

/* Adjacency information for row transitions */
static int  adj_len[ROWS];
static int  adj[ROWS][ROWS];

/* Cached sequence values */
static seq_t *a_values = NULL;
static idx_t  cached_len = 0;      /* number of allocated elements */

/* Highest n that has been computed so far */
static idx_t max_computed = 0;
static bool initialized = false;

/* Build all 3^6 rows and fill the adjacency list.
   adj[u][k] is a row v that can follow row u in a valid array. */
static void init(void)
{
    int rows[ROWS][6];
    for (int i = 0; i < ROWS; ++i) {
        int v = i;
        for (int k = 0; k < 6; ++k) {
            rows[i][k] = v % 3;
            v /= 3;
        }
    }

    for (int u = 0; u < ROWS; ++u) {
        int len = 0;
        for (int v = 0; v < ROWS; ++v) {
            bool ok = true;
            for (int j = 0; j < 5; ++j) {
                int a1 = rows[u][j];
                int a2 = rows[u][j+1];
                int b1 = rows[v][j];
                int b2 = rows[v][j+1];
                int inc = 0;
                if (a2 > a1) ++inc;          /* top edge */
                if (b2 > a2) ++inc;          /* right edge */
                if (b1 > b2) ++inc;          /* bottom edge */
                if (a1 > b1) ++inc;          /* left edge */
                if (inc != 1) { ok = false; break; }
            }
            if (ok) adj[u][len++] = v;
        }
        adj_len[u] = len;
    }

    /* initialise dp_prev for arrays of height 1 (single row) */
    for (int i = 0; i < ROWS; ++i)
        dp_prev[i] = (seq_t)1;

    initialized = true;
}

/* Grow the table of a(n) values up to required n */
static void ensure_index(idx_t n)
{
    if (!initialized) init();

    if (n <= max_computed) return;      /* already computed */

    /* how many elements do we need in a_values?  indices 0..n */
    idx_t new_len = n + 1;              /* need n+1 elements (0 unused) */

    if (cached_len < new_len) {
        /* allocate new buffer */
#if defined(__cplusplus)
        seq_t *tmp = NEW seq_t[new_len];
        /* copy existing data */
        for (idx_t i = 0; i < cached_len; ++i)
            tmp[i] = a_values[i];
        /* free old buffer */
        if (a_values) DEL a_values;
        a_values = tmp;
        cached_len = new_len;
#else
        a_values = (seq_t*)realloc(a_values, new_len * sizeof(seq_t));
        cached_len = new_len;
#endif
    }

    /* compute values for heights from max_computed+2 up to n+1 */
    for (idx_t height = max_computed + 2; height <= n + 1; ++height) {
        /* clear dp_curr */
        for (int i = 0; i < ROWS; ++i) dp_curr[i] = (seq_t)0;

        /* transition from dp_prev to dp_curr */
        for (int u = 0; u < ROWS; ++u) {
            seq_t val = dp_prev[u];
            if (val == (seq_t)0) continue;
            int len = adj_len[u];
            for (int k = 0; k < len; ++k) {
                int v = adj[u][k];
                dp_curr[v] += val;
            }
        }

        /* sum all end states → a_value for this height */
        seq_t sum = (seq_t)0;
        for (int v = 0; v < ROWS; ++v) sum += dp_curr[v];
        a_values[height - 1] = sum;   /* height = (n+1) ⇒ index = height-1 */

        /* prepare for next iteration */
        for (int i = 0; i < ROWS; ++i) dp_prev[i] = dp_curr[i];
    }

    max_computed = n;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0, harmless */
    ensure_index(n);
    return a_values[n];
}
