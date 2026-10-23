/*
 * A170641  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for n < 49: a(n) = 2 + 6 * Σ_{k=0}^{n-1} a(k) for n ≥
 *            49: a(n) = 6 * Σ_{k=1}^{48} a(n-k) – 21 * a(n-49)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static state for memoisation */
static idx_t  cache_len     = 0;     /* size of the allocated array */
static seq_t *cache         = NULL;  /* array of computed terms */
static idx_t  last_computed = 0;     /* highest index that has been computed */
static seq_t  prefix_sum    = 0;     /* Σ_{k=0}^{i} a(k)  (computed so far) */
static seq_t  window_sum    = 0;     /* Σ of the last 48 terms (needed for recurrence) */
static int    initialized   = 0;

/* ------------------------------------------------------------------ */
/*  Compute the n‑th term of A170641 (0‑based)                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialise the cache on first call */
    if (!initialized) {
        cache_len = 1;
#ifdef __cplusplus
        cache = new seq_t[cache_len];
#else
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
#endif
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;                  /* a(0) = 1 */
        prefix_sum = cache[0];
        last_computed = 0;
        window_sum = 0;                       /* no window yet */
        initialized = 1;
    }

    /* grow the cache if necessary */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_len];
#else
        new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
#endif
        if (!new_cache) return (seq_t)0;      /* allocation failure */
        for (idx_t i = 0; i <= last_computed; ++i)
            new_cache[i] = cache[i];
        for (idx_t i = last_computed + 1; i < new_len; ++i)
            new_cache[i] = (seq_t)0;          /* uncomputed marker */

#ifdef __cplusplus
        delete [] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_len = new_len;
    }

    /* compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 48) {
            /* a(i) = 2 + 6 * Σ_{k=0}^{i-1} a(k)  (prefix_sum holds Σ_{k=0}^{i-1}) */
            val = (seq_t)2 + (seq_t)6 * prefix_sum;
        } else {
            /* a(i) = 6 * Σ_{k=1}^{48} a(i-k) – 21 * a(i-49)  */
            val = (seq_t)6 * window_sum - (seq_t)21 * cache[i - 49];
        }

        cache[i] = val;
        prefix_sum += val;                 /* update running sum */

        /* update window for recurrence */
        if (i >= 1 && i <= 48) {
            window_sum += val;             /* a(i) joins the window */
        }
        if (i >= 49) {
            /* slide the window: add current, drop a(i-48) */
            window_sum += val;             /* a(i) just computed */
            window_sum -= cache[i - 48];   /* term leaving the window */
        }

        last_computed = i;
    }

    return cache[n];
}
