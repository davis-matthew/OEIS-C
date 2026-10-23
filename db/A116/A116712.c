/*
 * A116712  Number of permutations of length n which avoid the patterns 231,
 *          3214, 4312.
 * https://oeis.org/A116712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) + b(n) where b(n) = 1 for
 *            n=1, -1 for n=2, 1 for n=4, else 0 (derived from the generating
 *            function A(x) = (x^4-x^2+x)/(2x^3+x^2-3x+1) )
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[0] = 0, cache[1] = a(1), ... */
static idx_t cache_size = 0;    /* highest n for which cache[n] is valid */
static size_t cache_cap = 0;    /* allocated capacity */

/* grow cache size if needed, keeping space for index 0 through n */
static void ensure_cache_size(idx_t n)
{
    if (n <= cache_size)
        return;

    /* need at least n+1 elements (including index 0) */
    size_t need = (size_t)n + 1;
    if (cache_cap < need)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < need)
            new_cap = need;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) /* allocation failed: terminate */
            exit(1);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialize new entries with 0 by default (realloc doesn't zero) */
    for (size_t i = cache_size; i < n; ++i)
        cache[i + 1] = 0; /* placeholders; actual values computed below */

    /* compute missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        seq_t a_nm1 = (i >= 1) ? cache[i - 1] : 0;
        seq_t a_nm2 = (i >= 2) ? cache[i - 2] : 0;
        seq_t a_nm3 = (i >= 3) ? cache[i - 3] : 0;

        seq_t tmp = (seq_t)3 * a_nm1;
        tmp -= a_nm2;
        tmp -= (seq_t)2 * a_nm3;

        /* add correction b(n) */
        if (i == 1)
            tmp += (seq_t)1;
        else if (i == 2)
            tmp -= (seq_t)1;
        else if (i == 4)
            tmp += (seq_t)1;

        cache[i] = tmp;
    }

    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return 0;

    ensure_cache_size(n);
    return cache[n];
}
