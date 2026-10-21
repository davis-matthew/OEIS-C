/*
 * A068336  a(1) = 1; a(n+1) = 1 + sum{k|n} a(k), sum is over the positive
 *          divisors, k, of n.
 * https://oeis.org/A068336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = 1 + Σ_{k|n} a(k)
 * Range:     u8 n <= 20, u16 n <= 102, u32 n <= 812, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static cache of already-computed values.  
   When a new n is requested the cache is enlarged if necessary and
   all missing terms up to n are filled using the definition.  For every
   n<current_cap a fast lookup is returned.  Computing a(n) requires
   enumerating the divisors of n‑1, which gives an O(n √n) algorithm
   that is easily fast enough for n≈10000.  */

#include <stdlib.h>   /* for size_t, realloc */
#include <stdint.h>   /* for uint64_t */

static seq_t *a_cache = NULL;     /* cached sequence values (1‑based) */
static size_t cache_cap = 0;      /* allocated size of a_cache */

/* enlarge the cache so that indices up to n are valid */
static inline void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < (size_t)(n + 1))
        new_cap *= 2;

    a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * (new_cap + 1));
    if (!a_cache)
        exit(1);  /* cannot allocate – abort */

    /* initialise the newly allocated part with zero (to mark “not computed”) */
    for (size_t i = cache_cap + 1; i <= new_cap; ++i)
        a_cache[i] = (seq_t)0;

    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* defined only for n≥1 */

    ensure_cache(n);

    /* if already known, return it immediately */
    if (a_cache[n] != (seq_t)0)
        return a_cache[n];

    /* compute missing entries in increasing order */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (a_cache[i] != (seq_t)0)   /* already computed elsewhere */
            continue;

        if (i == 1)
        {
            a_cache[1] = (seq_t)1;
            continue;
        }

        idx_t m = i - 1;          /* the argument of the divisor sum */
        seq_t sum = (seq_t)0;

        /* enumerate divisors of m */
        for (idx_t d = 1; (uint64_t)d * d <= m; ++d)
        {
            if (m % d == 0)
            {
                sum += a_cache[d];
                idx_t e = m / d;
                if (e != d)
                    sum += a_cache[e];
            }
        }

        a_cache[i] = (seq_t)1 + sum;
    }

    return a_cache[n];
}
