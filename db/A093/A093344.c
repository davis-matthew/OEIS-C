/*
 * A093344  a(n) = n! * Sum_{i=1..n} (1/i)*Sum_{j=0..i-1} 1/j!.
 * https://oeis.org/A093344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{i=1..n} (1/i) * Σ_{j=0..i-1} 1/j! (OEIS A093344)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the recurrence:   */
/*   a(0) = 0, c(0) = 1                                   */
/*   c(k) = k * c(k-1) + 1  (k ≥ 1)                        */
/*   a(k) = k * a(k-1) + c(k-1)  (k ≥ 1)                    */
/* where c(k) = k! * Σ_{j=0..k} 1/j! . This avoids negative
   intermediate values and works with unsigned seq_t. */

#include <stdlib.h>
#include <string.h>

static seq_t *a_cache = NULL;   /* cache for a(n) */
static seq_t *c_cache = NULL;   /* cache for c(n) = n! * Σ_{j=0..n} 1/j! */
static size_t cache_cap = 0;    /* number of allocated slots (≥1) */
static idx_t max_n_computed = 0; /* largest n for which a[n] and c[n] are valid */

/* Ensure the caches are large enough to hold indices up to n.
   The function expands the arrays if necessary, preserving
   existing values. */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)(n + 1);

    if (cache_cap == 0) {
        /* First allocation: at least one slot for index 0 */
        cache_cap = 1;
        a_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        c_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (a_cache == NULL || c_cache == NULL) {
            /* minimal error handling – abort if allocation fails */
            abort();
        }
        a_cache[0] = (seq_t)0;
        c_cache[0] = (seq_t)1;
    }

    if (need > cache_cap) {
        /* Expand the cache to the required size */
        seq_t *new_a = (seq_t *)malloc(need * sizeof(seq_t));
        seq_t *new_c = (seq_t *)malloc(need * sizeof(seq_t));
        if (new_a == NULL || new_c == NULL) {
            abort();
        }
        memcpy(new_a, a_cache, cache_cap * sizeof(seq_t));
        memcpy(new_c, c_cache, cache_cap * sizeof(seq_t));
        free(a_cache);
        free(c_cache);
        a_cache = new_a;
        c_cache = new_c;
        cache_cap = need;
    }

    /* Compute all values up to n that have not yet been computed. */
    for (idx_t k = max_n_computed + 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t prev_c = c_cache[k - 1];
        c_cache[k] = kk * prev_c + (seq_t)1;          /* c(k) = k * c(k-1) + 1 */
        seq_t prev_a = a_cache[k - 1];
        a_cache[k] = kk * prev_a + prev_c;            /* a(k) = k * a(k-1) + c(k-1) */
    }
    max_n_computed = n;
}

seq_t a(idx_t n)
{
    /* Lazy initialization and memoized computation */
    ensure_cache(n);
    return a_cache[n];
}
