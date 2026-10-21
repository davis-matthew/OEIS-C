/*
 * A238492  Number of partitions of n not containing 4*(number of parts) as a
 *          part.
 * https://oeis.org/A238492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = p(n) – Σ_{k=1}^{⌊n/4⌋} P(n–4k, k–1) * where p(n) is the
 *            ordinary partition number and P(m, r) * counts partitions of m
 *            into exactly r parts
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utility includes */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static cache for computed values */
static seq_t *p_cache = NULL;   /* ordinary partitions p[n] */
static seq_t *c_cache = NULL;   /* sum_{k} P(n-4k, k-1) */
static seq_t *a_cache = NULL;   /* result a[n]           */
static idx_t  cache_size = 0;   /* number of entries allocated (max_n+1) */

/* Recompute all tables up to (and including) max_n */
static void recompute_tables(idx_t max_n)
{
    size_t N   = (size_t)max_n;
    size_t sz  = N + 1;                     /* indices 0..N */
    size_t i, n, k, m, limit_m;

    /* Free any previous cache */
    if (p_cache) free(p_cache);
    if (c_cache) free(c_cache);
    if (a_cache) free(a_cache);

    /* Allocate new cache arrays */
    p_cache = (seq_t*)malloc(sz * sizeof(seq_t));
    c_cache = (seq_t*)malloc(sz * sizeof(seq_t));
    a_cache = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!p_cache || !c_cache || !a_cache) return; /* allocation failure */

    /* ---------- ordinary partition numbers p[n] ---------- */
    for (i = 0; i < sz; ++i) p_cache[i] = 0;
    p_cache[0] = 1;
    for (size_t s = 1; s <= N; ++s) {
        for (n = s; n <= N; ++n) {
            p_cache[n] += p_cache[n - s];
        }
    }

    /* ---------- compute c_cache: sum_{k} P(n-4k, k-1) ---------- */
    for (i = 0; i < sz; ++i) c_cache[i] = 0;

    /* prev array holds P(·, k-1) during the loop */
    seq_t *prev = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *curr = (seq_t*)malloc(sz * sizeof(seq_t));
    for (i = 0; i < sz; ++i) prev[i] = 0;
    prev[0] = 1;  /* P(0,0) = 1, others zero */

    for (k = 1; 4 * k <= N; ++k) {
        /* compute curr[n] = P(n, k) */
        for (n = 0; n < sz; ++n) {
            seq_t val = 0;
            if (n >= 1) val += prev[n - 1];
            if (n >= k) val += curr[n - k];
            curr[n] = val;
        }

        /* add contributions to c_cache for this k */
        limit_m = N - 4 * k;
        for (m = 0; m <= limit_m; ++m) {
            c_cache[4 * k + m] += prev[m];   /* prev holds P(m, k-1) */
        }

        /* swap prev and curr for next iteration */
        seq_t *tmp = prev; prev = curr; curr = tmp;
    }

    free(prev);
    free(curr);

    /* ---------- compute a_cache = p - c ---------- */
    a_cache[0] = 0;
    for (n = 1; n < sz; ++n) {
        a_cache[n] = p_cache[n] - c_cache[n];
    }

    cache_size = sz;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 */
    if ((size_t)n + 1 > cache_size)
        recompute_tables(n);
    return a_cache[(size_t)n];
}
