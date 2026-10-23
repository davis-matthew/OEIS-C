/*
 * A197953  a(n) = 1 + Sum_{d|n, d>1} d * a(n/d).
 * https://oeis.org/A197953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{d|n , d>1} d * a(n/d) (offset 1, a(1)=1)
 * Range:     u8 n <= 23, u16 n <= 191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized cache of already‑computed values. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure that cache has indices 0 … n and that all values up to n are computed. */
static void grow_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_size)
        return;                      /* already sufficient */

    idx_t old_size = cache_size;      /* highest already stored index + 1 */

    /* Reallocate memory to fit indices 0 … n. */
    seq_t *tmp = (seq_t*)realloc(cache,
                                 (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failure */
    cache     = tmp;
    cache_size = n + 1;

    /* Base case a(1) = 1.  It is safe to set it again. */
    if (cache_size > 1)
        cache[1] = (seq_t)1;

    /* Compute all new entries in ascending order. */
    for (idx_t m = old_size; m <= n; ++m) {
        /* Term for divisor d = m (i.e. d = m, n/d = 1). */
        seq_t sum = (seq_t)m * cache[1];

        /* Iterate proper divisors d  (2 ≤ d ≤ √m). */
        for (idx_t d = 2; d * d <= m; ++d) {
            if (m % d == 0) {
                idx_t e = m / d;              /* complementary divisor */
                sum += (seq_t)d * cache[e];
                if (e != d)
                    sum += (seq_t)e * cache[d];
            }
        }
        cache[m] = sum + (seq_t)1;          /* a(m) = sum + 1 */
    }
}

/* Return the nth term of A197953 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* unused but harmless */

    /* Initialise cache on first call. */
    if (!cache) {
        cache = (seq_t*)calloc(2, sizeof(seq_t)); /* indices 0 and 1 */
        if (!cache) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    grow_cache(n);
    return cache[n];
}
