/*
 * A194816  Number of integers k in [1,n] such that {n*r+k*r} > {n*r-k*r},
 *          where { } = fractional part and r=sqrt(2).
 * https://oeis.org/A194816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = |{ k : 1 ≤ k ≤ n , { (n+k)*√2 } > { (n−k)*√2 } }|
 *
 * The function precomputes floor(j*√2) for j = 0 … 2n using an
 * integer‑only algorithm based on the identity
 *   floor(j*√2) = floor(√(2*j*j)).
 * For j from 1 upward the value is obtained from the previous entry
 * by either adding 1 or 2, chosen by a 128‑bit comparison that avoids
 * floating‑point arithmetic.  The array is grown on demand and cached
 * statically so that calls with arbitrary n remain O(n) per call.
 */

#include <stdint.h>
#include <stdlib.h>

/* static cache for floor(j*√2); stored as unsigned 64‑bit integers   */
static idx_t *cache_b = NULL;     /* B[j] = floor(j*√2) for j = 0 … 2n    */
static idx_t cache_b_size = 0;    /* number of entries in cache_b        */

/* Ensure the cache holds values up to max_index (inclusive).          */
static void ensure_cache_b(idx_t max_index)
{
    if (max_index + 1 <= cache_b_size)
        return;                                     /* already enough */

    idx_t new_size = max_index + 1;                  /* indices 0 … max_index */
    idx_t *new_b = (idx_t *)malloc(sizeof(idx_t) * new_size);
    if (!new_b)                                     /* allocation failure */
        return;

    /* Copy old values (if any) */
    if (cache_b) {
        for (idx_t i = 0; i < cache_b_size; ++i)
            new_b[i] = cache_b[i];
    }

    /* Compute new entries */
    if (new_size > 0)
        new_b[0] = 0;                               /* floor(0*√2)=0 */
    for (idx_t j = (cache_b_size > 0 ? cache_b_size : 1);
         j < new_size; ++j) {
        idx_t prev = new_b[j - 1];                  /* floor((j‑1)*√2) */
        idx_t cand = prev + 1;                      /* first possible value */

        /* limit = 2 * j^2, used for the 128‑bit comparison */
        unsigned __int128 limit = (unsigned __int128)j * j * 2;
        unsigned __int128 v1 = (unsigned __int128)cand * cand;

        idx_t val = cand;                           /* default candidate */
        if (v1 <= limit) {
            /* check whether cand + 1 is also valid */
            idx_t cand2 = cand + 1;
            unsigned __int128 v2 = (unsigned __int128)cand2 * cand2;
            if (v2 <= limit)
                val = cand2;
        }
        new_b[j] = val;                             /* floor(j√2) */
    }

    /* Replace old cache */
    if (cache_b)
        free(cache_b);
    cache_b       = new_b;
    cache_b_size  = new_size;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined by OEIS, safe fallback */

    idx_t needed = 2 * n;            /* need values up to index 2n   */
    ensure_cache_b(needed);
    if (!cache_b)                    /* allocation failure */
        return (seq_t)0;

    seq_t cnt = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t diff = cache_b[n + k] - cache_b[n - k];  /* floor((n+k)√2)-floor((n-k)√2) */
        idx_t fk   = cache_b[2 * k];                  /* floor(2k√2) = floor(k√8)       */
        if (fk >= diff)
            ++cnt;
    }
    return cnt;
}
