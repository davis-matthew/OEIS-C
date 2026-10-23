/*
 * A199699  Number of -3..3 arrays x(0..n-1) of n elements with zero sum and
 *          no two neighbors equal.
 * https://oeis.org/A199699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n arrays over {-3,…,3} with zero sum *
 *            and no adjacent equal elements. Computed by the DP described
 *            above
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**
 * Sequence A199699: Number of arrays of length n with elements in {-3,…,3},
 * zero sum, and no two adjacent elements equal.
 *
 * This implementation uses dynamic programming over the running sum.
 * For each length m we keep:
 *   f[sum]   – total number of sequences of length m with that sum
 *   g[v][sum] – number of such sequences ending with value v (-3≤v≤3)
 * Transition: append value v to a sequence of length m-1:
 *   new_sum = old_sum + v
 *   new_count = f[old_sum] – g[v][old_sum]   (cannot repeat v)
 *   f_new[new_sum] += new_count
 *   g_new[v][new_sum] = new_count
 *
 * Time complexity:  O(n²) operations (≈21·n² for n≤10000)
 * Memory: 8·(6·n+1) seq_t objects per call.
 *
 * The function is defined with external linkage and conforms to the
 * build environment requirements.
 */

#include <stdlib.h>
#include <stdbool.h>


seq_t a(idx_t n)
{
    if (n < 1)   /* undefined for n = 0, but return 0 safely */
        return (seq_t)0;

    /* constants for the value set */
    enum { VALS = 7 };      /* values -3 .. 3 */
    const int value_offset[VALS] = { -3, -2, -1, 0, 1, 2, 3 };

    /* initial step (m = 1) */
    size_t m = 1;
    size_t range_prev = 7;             /* 6*1 + 1 */
    size_t offset_prev = 3;            /* 3*1, for sum 0 -> index 3 */

    seq_t *f_prev = (seq_t *)calloc(range_prev, sizeof(seq_t));
    seq_t *g_prev[VALS];
    for (int v = 0; v < VALS; ++v) {
        g_prev[v] = (seq_t *)calloc(range_prev, sizeof(seq_t));
    }

    /* initialize with length 1 sequences */
    for (int v = 0; v < VALS; ++v) {
        size_t idx = (size_t)v;                 /* sum = v-3, index = sum+offset_prev */
        f_prev[idx] = (seq_t)1;
        g_prev[v][idx] = (seq_t)1;
    }

    /* iterate for m = 2 .. n */
    for (idx_t im = 2; im <= n; ++im) {
        m = (size_t)im;
        size_t range_curr = 6 * m + 1;
        size_t offset_curr = 3 * m;

        seq_t *f_curr = (seq_t *)calloc(range_curr, sizeof(seq_t));
        seq_t *g_curr[VALS];
        for (int v = 0; v < VALS; ++v) {
            g_curr[v] = (seq_t *)calloc(range_curr, sizeof(seq_t));
        }

        /* transition */
        for (int v = 0; v < VALS; ++v) {
            seq_t *prevg = g_prev[v];
            for (size_t idx = 0; idx < range_prev; ++idx) {
                seq_t temp = f_prev[idx];
                temp -= prevg[idx];
                size_t dest = idx + (size_t)v;   /* shift by v (0..6) */
                f_curr[dest] += temp;
                g_curr[v][dest] = temp;
            }
        }

        /* free previous arrays */
        free(f_prev);
        for (int v = 0; v < VALS; ++v) free(g_prev[v]);

        /* prepare for next iteration */
        f_prev = f_curr;
        range_prev = range_curr;
        offset_prev = offset_curr;
        for (int v = 0; v < VALS; ++v) g_prev[v] = g_curr[v];
    }

    /* answer: number of sequences of length n with sum 0 */
    seq_t ans = f_prev[3 * n];

    /* free remaining arrays */
    free(f_prev);
    for (int v = 0; v < VALS; ++v) free(g_prev[v]);

    return ans;
}
