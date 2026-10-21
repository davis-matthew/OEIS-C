/*
 * A081048  a(n) = Stirling1(n+1, 2) where Stirling1 is A048994.
 * https://oeis.org/A081048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) + (2*n-1)*a(n-1) + (n-1)^2*a(n-2) = 0,
   a(0) = 0, a(1) = 1.  Computes Stirling1(n+1,2). */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size of cache */
static size_t cache_len = 0;   /* highest index with a defined value */

/* Ensure that the cache can hold indices up to n inclusive.  Allocate
   or grow as necessary and initialise base values if created for
   the first time. */
static void ensure_cache_capacity(idx_t n)
{
    /* Need at least two slots for the base values. */
    size_t needed = (size_t)n + 1;
    if (needed < 2) needed = 2;

    if (cache == NULL) {
        cache_cap = needed;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 1;         /* values for 0 and 1 are set */
    } else if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
        /* Base values already present; no change needed. */
    }
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    ensure_cache_capacity(n);

    /* Return already computed value. */
    if (n <= cache_len) return cache[n];

    /* Compute missing values up to n using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* coefficients as seq_t to avoid overflow of idx_t. */
        seq_t coeff1 = (seq_t)2 * (seq_t)i - (seq_t)1;       /* 2*i-1   */
        idx_t im1 = i - 1;
        seq_t coeff2 = (seq_t)im1 * (seq_t)im1;              /* (i-1)^2 */

        seq_t term1 = cache[i - 1] * coeff1;
        seq_t term2 = cache[i - 2] * coeff2;
        cache[i] = -(term1 + term2);
    }

    cache_len = n;
    return cache[n];
}
