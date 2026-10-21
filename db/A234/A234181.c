/*
 * A234181  Number of (n+1)X(5+1) 0..5 arrays with every 2X2 subblock having
 *          the absolute values of all six edge and diagonal differences no
 *          larger than 1.
 * https://oeis.org/A234181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computation of OEIS sequence A234181:  
   a(n) = number of (n+1)×6 arrays with entries 0..5 
   in which every 2×2 subblock has all edge and diagonal differences 
   of absolute value ≤ 1.  
   The values are obtained by dynamic programming on the admissible
   rows.  The algorithm is described in the comments below. 
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#if defined(__cplusplus)
#include <cstddef>
#endif

/* 
   The typedefs seq_t and idx_t are supplied by the build system; 
   they must not be redefined here.  seq_t may be a builtin integer
   or an arbitrary‑precision integer class (C++ only).  idx_t is
   unsigned, usually uint64_t. 
*/

/* ---------- Global data for the DP ---------------------------------- */
static unsigned char (*rows)[6] = NULL;      /* admissible rows */
static int rows_count = 0;                   /* number of rows */
static int *edges_start = NULL;              /* start index of outgoing edges for each row */
static int *edges_len   = NULL;              /* number of outgoing edges for each row */
static int *edges_dest  = NULL;              /* destination row for each directed edge */

/* ---------- Utility .................................................. */
static inline bool rows_compatible(const unsigned char *a, const unsigned char *b)
{
    int diff, i;
    /* vertical differences */
    for (i = 0; i < 6; ++i) {
        diff = (int)a[i] - (int)b[i];
        if (diff < 0) diff = -diff;
        if (diff > 1) return false;
    }
    /* diagonal differences */
    for (i = 0; i < 5; ++i) {
        diff = (int)a[i] - (int)b[i + 1];
        if (diff < 0) diff = -diff;
        if (diff > 1) return false;
        diff = (int)a[i + 1] - (int)b[i];
        if (diff < 0) diff = -diff;
        if (diff > 1) return false;
    }
    return true;
}

/* ---------- One‑time initialization --------------------------------- */
static void ensure_init(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* generate all admissible rows: length‑6 sequences with
       consecutive elements differing by at most 1 */
    rows_count = 0;
    rows = (unsigned char (*)[6])malloc(1000 * sizeof(*rows));  /* 1000 > expected 950 */
    unsigned char r[6];
    for (unsigned int v = 0; v < 46656U; ++v) {
        unsigned int t = v;
        for (int k = 5; k >= 0; --k) {
            r[k] = t % 6U;
            t /= 6U;
        }
        bool ok = true;
        for (int k = 0; k < 5 && ok; ++k) {
            int d = (int)r[k] - (int)r[k + 1];
            if (d < 0) d = -d;
            if (d > 1) ok = false;
        }
        if (!ok) continue;
        for (int k = 0; k < 6; ++k)
            rows[rows_count][k] = r[k];
        ++rows_count;
    }

    /* build adjacency lists (directed) */
    edges_len = (int *)calloc(rows_count, sizeof(int));
    for (int j = 0; j < rows_count; ++j) {
        for (int i = 0; i < rows_count; ++i) {
            if (rows_compatible(rows[j], rows[i]))
                ++edges_len[j];
        }
    }

    edges_start = (int *)malloc(rows_count * sizeof(int));
    edges_start[0] = 0;
    for (int i = 1; i < rows_count; ++i)
        edges_start[i] = edges_start[i - 1] + edges_len[i - 1];

    int edges_total = edges_start[rows_count - 1] + edges_len[rows_count - 1];
    edges_dest = (int *)malloc(edges_total * sizeof(int));

    int *cur = (int *)malloc(rows_count * sizeof(int));
    for (int i = 0; i < rows_count; ++i) cur[i] = edges_start[i];

    for (int j = 0; j < rows_count; ++j) {
        for (int i = 0; i < rows_count; ++i) {
            if (rows_compatible(rows[j], rows[i]))
                edges_dest[cur[j]++] = i;
        }
    }
    free(cur);
}

/* ---------- Public routine ------------------------------------------ */
seq_t a(idx_t n)
{
    /* only defined for n >= 1 */
    if (n < 1) return 0;

    ensure_init();                      /* build tables if necessary */

    /* DP arrays: counts of ways to reach each row after a given number of rows */
#if defined(__cplusplus)
    seq_t *dp_curr = new seq_t[rows_count];
    seq_t *dp_next = new seq_t[rows_count];
#else
    seq_t *dp_curr = (seq_t *)malloc(rows_count * sizeof(seq_t));
    seq_t *dp_next = (seq_t *)malloc(rows_count * sizeof(seq_t));
#endif

    /* start with one occurrence of each admissible row */
    for (int i = 0; i < rows_count; ++i)
        dp_curr[i] = 1;

    /* iterate over the additional rows (n times) */
    for (int step = 1; step <= (int)n; ++step) {
        for (int i = 0; i < rows_count; ++i)
            dp_next[i] = 0;

        for (int i = 0; i < rows_count; ++i) {
            seq_t val = dp_curr[i];
            if (val == 0) continue;
            int start = edges_start[i];
            int end   = start + edges_len[i];
            for (int k = start; k < end; ++k) {
                int dest = edges_dest[k];
                dp_next[dest] += val;
            }
        }
        /* swap dp arrays */
        seq_t *tmp = dp_curr;
        dp_curr = dp_next;
        dp_next = tmp;
    }

    /* sum over all ending rows to obtain final count */
    seq_t ans = 0;
    for (int i = 0; i < rows_count; ++i)
        ans += dp_curr[i];

#if defined(__cplusplus)
    delete[] dp_curr;
    delete[] dp_next;
#else
    free(dp_curr);
    free(dp_next);
#endif

    return ans;
}
