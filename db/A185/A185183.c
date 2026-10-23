/*
 * A185183  G.f. A(x) satisfies A(x) = 1+x + x^2*[d/dx A(x)^2].
 * https://oeis.org/A185183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=1, a(n) = (n-1) * Σ_{k=0}^{n-1} a(k) * a(n-1-k) for n
 *            ≥ 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of computed terms */
static idx_t  cache_max = 0;    /* highest n for which cache[n] is valid */
static size_t cache_cap = 0;    /* allocated size of cache (≥ cache_max+1) */

/* ensure that a(0) … a(n) are stored in cache */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max)            /* already available */
        return;

    /* ---------- allocation / reallocation ---------- */
    if (cache == NULL) {           /* first call: allocate */
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);

        /* initialise the known base cases */
        cache[0] = (seq_t)1;
        if (cache_cap > 1)
            cache[1] = (seq_t)1;
        cache_max = 1;
    } else if ((size_t)(n + 1) > cache_cap) { /* grow */
        cache_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
    }

    /* ---------- compute missing values ---------- */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k) {
            sum += cache[k] * cache[m - 1 - k];
        }
        cache[m] = (seq_t)(m - 1) * sum;
    }
    cache_max = n;
}

/* public entry point for the sequence */
seq_t a(idx_t n)
{
    if (n <= 1)                       /* a(0) and a(1) */
        return (seq_t)1;
    ensure_cache(n);
    return cache[n];
}
