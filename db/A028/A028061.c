/*
 * A028061  Expansion of 1/((1-3x)(1-5x)(1-7x)(1-10x)).
 * https://oeis.org/A028061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*10^(n+3)-5*7^(n+4)+42*5^(n+3)-5*3^(n+4))/840
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  recurrence derived from the generating function 1/((1-3x)(1-5x)(1-7x)(1-10x));
    a(n) = 25*a(n-1) - 221*a(n-2) + 815*a(n-3) - 1050*a(n-4)  for n≥4
    with a(0)=1, a(1)=25, a(2)=404, a(3)=5390. */

#include <stdlib.h>

static seq_t *cache = NULL;       /* cached values */
static idx_t cache_len = 0;       /* number of computed terms */
static size_t cache_cap = 0;      /* allocated capacity */

/* ensure the cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* compute a(n) using memoised recurrence */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)25;
        else if (i == 2)
            cache[i] = (seq_t)404;
        else if (i == 3)
            cache[i] = (seq_t)5390;
        else
        {
            seq_t val =
                (seq_t)25 * cache[i - 1]
              - (seq_t)221 * cache[i - 2]
              + (seq_t)815 * cache[i - 3]
              - (seq_t)1050 * cache[i - 4];
            cache[i] = val;
        }
    }

    cache_len = n + 1;
    return cache[n];
}
