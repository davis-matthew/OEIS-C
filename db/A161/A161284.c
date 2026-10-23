/*
 * A161284  Number of partitions of n into numbers not divisible by 3 where
 *          every part appears at least 6 times.
 * https://oeis.org/A161284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 185, u32 n <= 533, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A161284: Number of partitions of n into numbers
   not divisible by 3 where every part appears at least 6 times. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"

static seq_t *dp = NULL;
static idx_t  dp_nmax = 0;

static void recompute(idx_t N)
{
    if (N < 1) return;

    /* Allocate working tables */
    seq_t *f = alloc_seq_array(N + 1);   /* current DP table */
    seq_t *g = alloc_seq_array(N + 1);   /* next DP table */
    seq_t *I = alloc_seq_array(N + 1);   /* temporary array for sums */

    if (!f || !g || !I) { free(f); free(g); free(I); return; }

    /* initialise the empty partition */
    for (idx_t i = 0; i <= N; ++i) f[i] = 0;
    f[0] = 1;

    for (idx_t s = 1; s <= N; ++s) {
        if (s % 3 == 0) continue; /* skip multiples of 3 */

        /* I[n] = sum_{k=6}^{⌊n/s⌋} f[n - k*s]   (recurrence)
           I[n] = 0                           if n < 6*s
           I[n] = I[n-s] + f[n-6*s]           otherwise               */
        for (idx_t n = 0; n <= N; ++n) {
            if (n < 6 * s) {
                I[n] = 0;
            } else {
                I[n] = I[n - s] + f[n - 6 * s];
            }
        }

        /* compute new DP values */
        for (idx_t n = 0; n <= N; ++n) {
            g[n] = f[n] + I[n];
        }

        /* prepare for next part size */
        seq_t *tmp = f;
        f = g;
        g = tmp;
    }

    /* free temporary arrays */
    free(g);
    free(I);

    /* store new DP table */
    free(dp);
    dp = f;
    dp_nmax = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (dp_nmax < n) recompute(n);
    return dp[n];
}
