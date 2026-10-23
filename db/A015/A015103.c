/*
 * A015103  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=-7.
 * https://oeis.org/A015103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 7, i128 n <= 10,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n+1) = Σ_{i=0..n} (-7)^i * a(i) * a(n-i)   */
#include <stddef.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;           /* array of computed values   */
static size_t cache_cap = 0;           /* allocated storage size      */
static idx_t max_computed = 0;         /* last index for which cache[i] is known */
static int cache_initialized = 0;      /* flag to initialise cache    */

seq_t a(idx_t n)
{
    /* initialise the cache on first use */
    if (!cache_initialized) {
        /* start with space for one element */
        ensure_capacity(1, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[0] = 1;
        max_computed = 0;
        cache_initialized = 1;
    }

    /* grow the buffer if needed */
    if ((size_t)n + 1 > cache_cap)
        ensure_capacity((size_t)n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* compute any missing terms up to n */
    while (max_computed < n) {
        idx_t k = max_computed + 1;   /* compute a(k) */
        seq_t sum = 0;
        seq_t pow_q = 1;
        const seq_t Q = -7;

        for (idx_t i = 0; i < k; ++i) {
            seq_t prod = pow_q * cache[i] * cache[k - 1 - i];
            sum += prod;
            pow_q *= Q;
        }

        cache[k] = sum;
        max_computed = k;
    }

    return cache[n];
}
