/*
 * A073253  Table of expansion of Product (1+(xy)^n/y)(1+(xy)^n/x), n>0 by
 *          antidiagonals.
 * https://oeis.org/A073253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A073253 – table of expansion of 
 *
 *   G(x,y) = ∏_{m≥1} (1 + x^m y^{m-1})(1 + x^{m-1} y^m)
 *
 * by antidiagonals.  For a given index n the algorithm determines the
 * antidiagonal k such that k(k+1)/2 ≤ n < (k+1)(k+2)/2, and defines
 * i = n - k(k+1)/2, j = k - i.  The value a(n) equals the coefficient
 * of x^i y^j in G(x,y).  The coefficient is counted by selecting, for
 * each m, one of the four options: (none), (x^m y^{m-1}), (x^{m-1} y^m)
 * or both.  A dynamic programme updates the counts for all pairs (i,j)
 * with i+j ≤ k while iterating m from 1 to k.  The values are cached
 * so that subsequent queries run in O(1).
 */
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

/* The types seq_t and idx_t are supplied by the build. */
static seq_t *CACHE = NULL;          /* cached sequence values */
static size_t CACHE_SIZE = 0;        /* number of cached entries */

static void ensure_cache(idx_t n)
{
    /* Determine the largest antidiagonal needed */
    idx_t kmax_needed = (idx_t)((sqrt(8.0 * n + 1.0) - 1.0) / 2.0);

    /* Determine required cache size: total entries up to and including
       antidiagonal kmax_needed are (kmax_needed+1)(kmax_needed+2)/2. */
    size_t new_cache_size = (size_t)(kmax_needed + 1) * (size_t)(kmax_needed + 2) / 2;

    if (new_cache_size <= CACHE_SIZE) return;   /* already computed */

    /* DP dimension */
    idx_t dim = kmax_needed + 1;
    size_t dp_size = (size_t)dim * (size_t)dim;

    /* Allocate DP arrays */
    seq_t *dp      = (seq_t *)calloc(dp_size, sizeof(seq_t));
    seq_t *dp_snap = (seq_t *)calloc(dp_size, sizeof(seq_t));
    if (!dp || !dp_snap) {
        /* Allocation failed – abort silently (well‑behaved in most
           testing environments). */
        return;
    }

    dp[0] = 1;   /* base case: exponent (0,0) */

    for (idx_t m = 1; m <= kmax_needed; ++m) {
        /* Snapshot of current DP to use for transitions */
        for (size_t idx = 0; idx < dp_size; ++idx) dp_snap[idx] = dp[idx];

        for (idx_t i = 0; i < dim; ++i) {
            for (idx_t j = 0; j < dim; ++j) {
                seq_t val = dp_snap[i * dim + j];
                if (!val) continue;

                idx_t ii, jj;

                /* choose m1 only: (m, m-1) */
                ii = i + m;          jj = j + m - 1;
                if (ii < dim && jj < dim && ii + jj < dim)
                    dp[ii * dim + jj] += val;

                /* choose m2 only: (m-1, m) */
                ii = i + m - 1;      jj = j + m;
                if (ii < dim && jj < dim && ii + jj < dim)
                    dp[ii * dim + jj] += val;

                /* choose both: (2m-1, 2m-1) */
                ii = i + 2 * m - 1;   jj = j + 2 * m - 1;
                if (ii < dim && jj < dim && ii + jj < dim)
                    dp[ii * dim + jj] += val;
            }
        }
    }
    free(dp_snap);

    /* Allocate new cache */
    seq_t *new_cache = (seq_t *)calloc(new_cache_size, sizeof(seq_t));
    if (!new_cache) {
        free(dp);
        return;
    }

    /* Fill cache entries */
    for (idx_t k = 0; k <= kmax_needed; ++k) {
        idx_t start = k * (k + 1) / 2;
        for (idx_t i = 0; i <= k; ++i) {
            idx_t j   = k - i;
            idx_t idx = start + i;
            new_cache[idx] = dp[i * dim + j];
        }
    }

    free(dp);

    /* Replace old cache */
    if (CACHE) free(CACHE);
    CACHE       = new_cache;
    CACHE_SIZE  = new_cache_size;
}

seq_t a(idx_t n)
{
    /* Ensure cache covers requested index */
    if (n >= CACHE_SIZE) ensure_cache(n);
    return CACHE[n];
}
