/*
 * A200455  Number of -n..n arrays x(0..2) of 3 elements with zero sum and
 *          nonzero first and second differences.
 * https://oeis.org/A200455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4), with initial values a(1)=4,
 *            a(2)=10, a(3)=26, a(4)=44. (1‑based indexing, a(1) is the first
 *            term.)
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the out‑of‑range value; callers always use n ≥ 1. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache for already computed terms.
       cache[i] holds a(i+1). */
    static seq_t *cache     = NULL;
    static idx_t  cache_len = 0;   /* number of valid terms in the cache   */
    static idx_t  cache_cap = 0;   /* current allocation size (≥ cache_len) */

    /* First initialisation: create a buffer large enough for the requested
       term, but only initialise the first four values. */
    if (cache == NULL)
    {
        idx_t init_cap = (n > 4) ? n : 4;          /* allocate at least 4 slots */
        cache = (seq_t *)realloc(NULL, init_cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;                       /* allocation failure */

        /* Base cases */
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)26;
        cache[3] = (seq_t)44;

        cache_len = 4;      /* four terms are now stored */
        cache_cap = init_cap;
    }
    /* If a larger capacity is requested, grow the buffer. */
    else if (n > cache_cap)
    {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (tmp == NULL)
            return cache[cache_len - 1];   /* keep old cache on failure */
        cache = tmp;
        cache_cap = n;
    }

    /* If the requested term is already known, return it immediately. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute missing terms up to n, using the recurrence. */
    for (idx_t i = cache_len; i < n; ++i)   /* i is 0‑based index of a(i+1) */
    {
        /* a(i+1) = 2*a(i) - 2*a(i-2) + a(i-3)   (indices in cache array) */
        seq_t res = cache[i - 1];   /* a(i)          */
        res += cache[i - 1];        /* 2*a(i)        */

        seq_t t2 = cache[i - 3];    /* a(i-2)        */
        t2 += cache[i - 3];        /* 2*a(i-2)      */

        res -= t2;                 /* 2*a(i) - 2*a(i-2) */
        res += cache[i - 4];       /* + a(i-3)           */

        cache[i] = res;            /* store a(i+1)  */
    }

    cache_len = n;                   /* all terms up to n are now known */
    return cache[n - 1];
}
