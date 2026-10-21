/*
 * A092268  Total number of smallest parts in all partitions of n into odd
 *          parts.
 * https://oeis.org/A092268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = total number of smallest parts in all partitions of n
 *            into odd parts
 * Range:     u8 n <= 18, u16 n <= 56, u32 n <= 190, u64 n <= 687,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* external linkage expected by the build system */
seq_t a(idx_t n)
{
    static seq_t *a_vals = NULL;   /* a[n] for 0..max_n   */
    static seq_t *g_prev = NULL;   /* g[n] = #partitions of n into odd parts >= current s+2 (previous step) */
    static seq_t *g_cur  = NULL;   /* temporary array */
    static seq_t *D      = NULL;   /* intermediate array */
    static seq_t *E      = NULL;   /* intermediate array */
    static idx_t  max_n  = 0;      /* largest n computed so far */

    idx_t N = n;
    if (N == 0)
        return (seq_t)0;           /* not used but safe */

    /* Allocate or grow the arrays if needed */
    if (N > max_n) {
        idx_t sz = N + 1;          /* include index 0 */

        /* Allocate new arrays */
        seq_t *new_a   = (seq_t*)malloc(sz * sizeof(seq_t));
        seq_t *new_gp  = (seq_t*)malloc(sz * sizeof(seq_t));
        seq_t *new_gc  = (seq_t*)malloc(sz * sizeof(seq_t));
        seq_t *new_D   = (seq_t*)malloc(sz * sizeof(seq_t));
        seq_t *new_E   = (seq_t*)malloc(sz * sizeof(seq_t));

        if (!new_a || !new_gp || !new_gc || !new_D || !new_E) {
            /* allocation failed – abort */
            exit(1);
        }

        /* initialise all to zero */
        for (idx_t i = 0; i < sz; ++i) {
            new_a[i] = (seq_t)0;
            new_gp[i] = (seq_t)0;
            new_gc[i] = (seq_t)0;
            new_D[i] = (seq_t)0;
            new_E[i] = (seq_t)0;
        }

        /* free old arrays */
        free(a_vals); free(g_prev); free(g_cur); free(D); free(E);

        a_vals = new_a;
        g_prev = new_gp;
        g_cur  = new_gc;
        D      = new_D;
        E      = new_E;
        max_n  = N;
    }

    /* Compute up to max_n if not already computed */
    if (N > max_n) {
        /* This case cannot happen because we resized above, but kept for safety */
    }

    /* Initialise dp arrays */
    g_prev[0] = (seq_t)1;          /* g[0] = 1 (empty partition) */
    for (idx_t i = 1; i <= N; ++i)
        g_prev[i] = (seq_t)0;

    /* Main DP over odd s values, descending */
    for (idx_t s = (N & 1) ? N : N - 1; ; ) {
        /* Compute D and E using current g_prev (which is g[n][s+2]) */
        D[0] = g_prev[0];
        E[0] = (seq_t)0;
        for (idx_t n = 1; n <= N; ++n) {
            if (n >= s) {
                D[n] = g_prev[n] + D[n - s];
                E[n] = E[n - s] + D[n - s];
            } else {
                D[n] = g_prev[n];
                E[n] = (seq_t)0;
            }
        }

        /* Accumulate contributions to a[n] (sum of multiplicities) */
        for (idx_t n = s; n <= N; ++n) {
            a_vals[n] = a_vals[n] + E[n];
        }

        /* Compute g_cur = partitions with parts >= s */
        for (idx_t i = 0; i <= N; ++i)
            g_cur[i] = g_prev[i];
        for (idx_t n = s; n <= N; ++n) {
            g_cur[n] = g_cur[n] + g_cur[n - s];
        }

        /* Prepare for next iteration: g_prev becomes g_cur */
        seq_t *tmp = g_prev;
        g_prev = g_cur;
        g_cur  = tmp;

        if (s == 1)
            break;          /* last odd s processed */
        s -= 2;
    }

    return a_vals[N];
}
