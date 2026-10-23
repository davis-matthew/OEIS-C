/*
 * A028677  Pseudo Galois numbers for d=15.
 * https://oeis.org/A028677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15^n * Product_{k=0}^{n-1} (15^n - 15^k)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent form used for efficient incremental computation:
 * a(n) = 15^{n(n+1)/2} * ∏_{j=1}^{n} (15^j - 1) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for incremental computation */
    static idx_t last_n = 0;                      /* largest n computed so far */
    static seq_t pow15   = 1;                    /* 15^last_n */
    static seq_t prodB   = 1;                    /* ∏_{j=1}^{last_n} (15^j - 1) */
    static seq_t powFact = 1;                    /* 15^{last_n*(last_n+1)/2} */
    static seq_t *cache = NULL;                  /* cache of computed a(n) */
    static size_t cache_cap = 0;                 /* allocated capacity of cache */

    /* initialise cache for n = 0 */
    if (cache == NULL)
    {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) /* allocation failed – return zero as safe fallback */
            return (seq_t)0;
        cache[0] = 1;     /* a(0) = 1 */
    }

    /* extend computation up to requested n */
    while (last_n < n)
    {
        idx_t next = last_n + 1;

        /* compute 15^next */
        pow15 = pow15 * 15;

        /* update product of (15^j - 1) for j = 1..next */
        seq_t term = pow15 - 1;
        prodB = prodB * term;

        /* update power factor 15^{next*(next+1)/2} */
        powFact = powFact * pow15;

        /* ensure cache capacity */
        if ((size_t)next >= cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= (size_t)next)
                new_cap = next + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) /* allocation failed – keep old cache */
                return 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* store a(next) */
        cache[next] = powFact * prodB;

        /* move to next */
        last_n = next;
    }

    return cache[n];
}
