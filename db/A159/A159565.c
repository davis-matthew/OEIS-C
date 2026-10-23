/*
 * A159565  Positive numbers y such that y^2 is of the form x^2+(x+241)^2 with
 *          integer x.
 * https://oeis.org/A159565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=221, a(2)=241, a(3)=265, a(4)=1061, a(5)=1205, a(6)=1369,
 *            a(n)=6*a(n-3)-a(n-6) for n>6
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* 1‑based index, cache[0] unused */
static size_t cache_cap = 0;     /* number of elements currently allocated (>=1) */
static idx_t cache_up_to = 0;    /* largest index that has been computed */

/* Initial values for indices 1..6 */
static const seq_t init_vals[7] = {0, 221, 241, 265, 1061, 1205, 1369};

/* Grow the cache to at least 'len' (1‑based) elements.  */
static void ensure_cache(size_t len)
{
    if (len <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < len)
        new_cap <<= 1;            /* double until big enough */

    /* realloc automatically frees the old buffer */
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp)
        abort();                 /* out of memory */

    cache = tmp;
    cache_cap = new_cap;
    /* Initialize new slots to zero (not strictly needed but safe) */
    for (size_t i = cache_cap; i <= new_cap; ++i)
        cache[i] = 0;
    cache[0] = 0;                /* dummy at position 0 */
}

/* Compute all terms up to n, storing them in the cache. */
static void compute_terms(idx_t n)
{
    if (n <= cache_up_to)
        return;

    /* Ensure we have at least up to n allocated. */
    ensure_cache((size_t)n);

    /* Fill memoisation for indices 1..6 if not already set. */
    for (idx_t i = 1; i <= 6; ++i)
        if (cache[i] == 0)
            cache[i] = init_vals[i];

    /* Compute missing terms in order. */
    for (idx_t i = cache_up_to + 1; i <= n; ++i) {
        if (i <= 6)
            cache[i] = init_vals[i];
        else
            cache[i] = 6 * cache[i - 3] - cache[i - 6];
    }

    cache_up_to = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n<1, not needed */

    if (cache_up_to < n)
        compute_terms(n);

    return cache[n];
}
