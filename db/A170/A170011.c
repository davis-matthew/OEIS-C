/*
 * A170011  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A170011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = p_n
 *           + 48 * sum_{k=1}^{min(34,n)} a(n-k)
 *           - 1176 * [n >= 35] * a(n-35),
 * where
 *   p_0 = 1,           p_1 … p_34 = 2,
 *   p_35 = 1,          p_n = 0 for n > 35.
 *
 * This follows from the rational generating function
 *   P(t) / Q(t)  with  Q(t) = 1 - 48 t - … - 48 t^34 + 1176 t^35.
 *
 * The implementation memoises computed terms in a static array.
 * It is thread‑safe only in a single thread context.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;   /* array of a(0)…a(cache_len-1)           */
static size_t cache_cap = 0;  /* allocated capacity of the array        */
static size_t cache_len = 0;  /* number of terms currently cached      */

/* Initialise the first 35 terms (indices 0..34).  Assumes cache has
 * capacity at least 35. */
static void init_initial_terms(void)
{
    cache[0] = (seq_t)1;                /* a(0) */
    for (size_t n = 1; n <= 34; ++n) {
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k < n; ++k)
            sum += cache[k];
        cache[n] = (seq_t)2 + ((seq_t)48 * sum);
    }
    cache_len = 35;                     /* indices 0..34 are now valid   */
}

/* Ensure that cache contains terms up to at least n.  If necessary,
 * grow the array and compute the missing terms using the recurrence. */
static void ensure_cache(size_t n)
{
    /* Allocate array if needed */
    if (cache == NULL) {
        cache_cap = n + 1;
        if (cache_cap < 35) cache_cap = 35;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap < n + 1) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 128;
        while (new_cap < n + 1) new_cap *= 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* Initialise the first 35 terms (if not already done) */
    if (cache_len == 0)
        init_initial_terms();

    /* Extend cache up to index n, if necessary */
    if (cache_len > n)
        return;                         /* nothing to do */

    for (size_t i = cache_len; i <= n; ++i) {
        seq_t sum;
        /* p_i term */
        if (i == 0)
            sum = (seq_t)1;
        else if (i <= 34)
            sum = (seq_t)2;
        else if (i == 35)
            sum = (seq_t)1;
        else
            sum = (seq_t)0;

        /* 48 * sum_{k=1}^{min(34,i)} a(i-k) */
        for (size_t j = 1; j <= 34; ++j)
            sum += ((seq_t)48 * cache[i - j]);

        /* -1176 * a(i-35) if i >= 35 */
        if (i >= 35)
            sum -= ((seq_t)1176 * cache[i - 35]);

        cache[i] = sum;
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    if (idx >= cache_len)
        ensure_cache(idx);
    return cache[idx];
}
