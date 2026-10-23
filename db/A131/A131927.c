/*
 * A131927  Expansion of series reversion of x * (1 - 9*x) / (1 - x).
 * https://oeis.org/A131927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 (n=0); a(1)=1; a(2)=8; For n ≥ 3: n*a(n) =
 *            17*(2*n-3)*a(n-1) – (n-3)*a(n-2) (derived from the convolution
 *            recurrence a(n) = –a(n-1) + 9*∑_{k=1}^{n-1} a(k)*a(n-k) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array holding computed a(i)  */
static size_t cache_size = 0;      /* allocated size of cache       */
static idx_t  max_computed = 2;    /* highest index that has been computed */

/* Compute a(n), with memoisation for any order of calls. */
seq_t a(idx_t n)
{
    /* Ensure cache is large enough to hold indices up to n. */
    size_t need = (size_t)n + 1;
    if (cache_size < need) {
        size_t new_size = need < 3 ? 3 : need;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();               /* out of memory */
        cache = tmp;

        /* Initialise the first three values if this is the first allocation. */
        if (cache_size == 0) {
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)8;
        }
        cache_size = new_size;
    }

    /* If the requested n has already been computed, just return it. */
    if (n <= max_computed)
        return cache[(size_t)n];

    /* Compute missing terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i < 3) { /* safety, should not happen */
            cache[(size_t)i] = (seq_t)i;
            continue;
        }

        /* Use the linear recurrence to avoid O(n^2) convolution. */
        seq_t term1 = (seq_t)17 *
                      (seq_t)(2 * i - 3) *
                      cache[(size_t)(i - 1)];
        seq_t term2 = (seq_t)(i - 3) *
                      cache[(size_t)(i - 2)];
        cache[(size_t)i] = (term1 - term2) / (seq_t)i;
    }

    max_computed = n;
    return cache[(size_t)n];
}
