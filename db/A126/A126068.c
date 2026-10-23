/*
 * A126068  Expansion of 1 - x - sqrt(1 - 2*x - 3*x^2) in powers of x.
 * https://oeis.org/A126068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 0,1; a(2) = 2; for n ≥ 3: a(n) = a(n-1) + (1/2)
 *            * Σ_{k=2}^{n-2} a(k) * a(n-k)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 46, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache = NULL;   /* cache[i] holds a(i) */
static idx_t  cache_size = 0;   /* first unused index in cache (i.e. equal to number of computed values) */
static size_t cache_cap   = 0;   /* allocated capacity */

/* Ensure that a_cache is filled for indices 0..n */
static void
ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return; /* already computed */

    /* Grow the cache if necessary */
    size_t need = (size_t)(n + 1);
    if (need > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed: abort further computation */
            return;
        }
        a_cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize new entries */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0 || i == 1) {
            a_cache[i] = 0;
        } else if (i == 2) {
            a_cache[i] = 2;
        } else {
            seq_t sum = 0;
            for (idx_t k = 2; k <= i - 2; ++k) {
                sum += a_cache[k] * a_cache[i - k];
            }
            a_cache[i] = a_cache[i - 1] + (sum >> 1); /* sum is even */
        }
    }
    cache_size = (idx_t)(n + 1);
}

seq_t
a(idx_t n)
{
    if (n == 0 || n == 1)
        return 0;

    ensure_cache(n);
    return a_cache[n];
}
