/*
 * A202563  Numbers which are both decagonal and pentagonal.
 * https://oeis.org/A202563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9602*a(n-1)-a(n-2)+2900 with initial values a(1)=1,
 *            a(2)=12376, a(3)=118837251. The function uses memoised iterative
 *            recurrence
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* 0‑based array of computed terms      */
static size_t cache_capacity = 0;  /* allocated size of `cache`          */
static size_t cache_max = 0;        /* highest index (1‑based) cached    */

/* ensure that cache can hold at least `n` terms (n>=1) */
static void ensure_capacity(size_t n)
{
    if (n <= cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity : 1;
    while (new_cap < n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();  /* out of memory */
    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;     /* undefined for n==0, return 0 as a safe value */

    /* initialise cache and first three terms if necessary */
    if (!cache) {
        size_t init_cap = n > 3 ? n : 3;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache)
            abort();  /* out of memory */
        cache[0] = (seq_t)1;
        if (init_cap > 1)
            cache[1] = (seq_t)12376;
        if (init_cap > 2)
            cache[2] = (seq_t)118837251;
        cache_capacity = init_cap;
        cache_max = init_cap;
    }

    /* if the requested term is not yet cached, compute up to it */
    if ((size_t)n > cache_max) {
        /* ensure capacity first */
        ensure_capacity((size_t)n);

        /* initialise previous two terms for the recurrence */
        seq_t prev2, prev1;
        if (cache_max >= 2) {
            prev2 = cache[cache_max - 2];   /* a(cache_max-1) */
            prev1 = cache[cache_max - 1];   /* a(cache_max)   */
        } else if (cache_max == 1) {
            prev2 = 0; /* unused */
            prev1 = cache[0];           /* a(1) */
        } else { /* cache_max == 0 -> needs initialisation */
            /* This case only occurs if n < 3 and cache not yet set */
            prev2 = 0;
            prev1 = 0;
        }

        /* compute missing terms up to n */
        for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
            /* a(i) = 9602*a(i-1) - a(i-2) + 2900 */
            seq_t term = (seq_t)9602 * prev1 - prev2 + (seq_t)2900;
            cache[i - 1] = term;
            prev2 = prev1;
            prev1 = term;
        }
        cache_max = (size_t)n;
    }

    return cache[n - 1]; /* 1‑based to 0‑based index */
}
