/*
 * A232131  Number of (n+1) X (2+1) 0..2 arrays with no element equal to a
 *          strict majority of its horizontal and antidiagonal neighbors, with
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A232131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) - 36*a(n-2) + 16*a(n-3) (n ≥ 4) Initial values:
 *            a(1)=36, a(2)=728, a(3)=14752 (Equation holds for all n ≥ 1;
 *            terms 1‑based) Computation uses a static cache and a linear
 *            recurrence
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* 1‑based cache, cache[0] unused    */
static size_t cache_cap = 0;   /* allocated size of cache array      */
static idx_t  computed  = 0;   /* largest n computed and stored      */

/* Ensure the cache can hold values up to index n (inclusive). */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) /* enough space already          */
        return;

    size_t new_cap = cache_cap ? cache_cap : 4;
    while ((size_t)n + 1 > new_cap)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache,
                                        new_cap * sizeof(seq_t));
    if (!new_cache) /* allocation failed – abort with zero */
        return;
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Return a(n) for n ≥ 1.  For n < 1 the function returns 0. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Initialize cache on first call */
    if (!cache) {
        ensure_cache_capacity(3);                /* need indices 1‑3 */
        cache[0] = (seq_t)0;                     /* unused slot */
        cache[1] = (seq_t)36;                    /* a(1) */
        cache[2] = (seq_t)728;                   /* a(2) */
        cache[3] = (seq_t)14752;                 /* a(3) */
        computed = 3;
    }

    if (n <= computed)            /* already known */
        return cache[n];

    /* Grow cache to required size */
    ensure_cache_capacity(n);

    /* Compute missing terms up to n using recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t t = cache[i - 1] * (seq_t)22;
        t -= cache[i - 2] * (seq_t)36;
        t += cache[i - 3] * (seq_t)16;
        cache[i] = t;
    }

    computed = n;
    return cache[n];
}
