/*
 * A196906  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,1,2,0,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=a(n-1)+2*a(n-2)+3*a(n-3)+2*a(n-4)-a(n-6) with a(1)=2,
 *            a(2)=6, a(3)=13, a(4)=34, a(5)=81, a(6)=199
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* All work fits into the supplied seq_t, using only integer
   arithmetic and the usual C/C++ operators. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache   = NULL;  /* cache for already computed terms   */
static size_t cache_cap = 0;   /* allocated length (>= n+1)          */
static size_t cache_last = 0;  /* highest index that has been filled */

static void init_cache(void)
{
    /* Allocate space for indices 0 .. 6 (we only use 1..6) */
    cache_cap = 7;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache)
        exit(1);
    /* base terms, 1‑based indexing */
    static const seq_t base[6] = {2, 6, 13, 34, 81, 199};
    for (size_t i = 1; i <= 6; ++i)
        cache[i] = base[i - 1];
    /* Term 0 is left untouched (it is not part of the recurrence) */
    cache_last = 6;
}

static void ensure_capacity(size_t n)
{
    /* need to address cache[n] (index n) */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 2;          /* keep one spare slot            */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* out of range, return 0          */

    if (!cache)
        init_cache();                   /* lazily initialize on first call */

    ensure_capacity((idx_t)n);

    if ((idx_t)n <= 6)                   /* known base case                */
        return cache[(size_t)n];

    /* compute missing terms up to n */
    for (size_t i = cache_last + 1; i <= (size_t)n; ++i) {
        if (i < 7) {                     /* (should not occur)              */
            /* copy base values if needed */
            static const seq_t base[6] = {2, 6, 13, 34, 81, 199};
            cache[i] = base[i - 1];
        } else {
            cache[i] = cache[i - 1]          /* a(i-1) */
                     + 2 * cache[i - 2]       /* 2*a(i-2) */
                     + 3 * cache[i - 3]       /* 3*a(i-3) */
                     + 2 * cache[i - 4]       /* 2*a(i-4) */
                     - cache[i - 6];          /* -a(i-6) */
        }
    }
    cache_last = (size_t)n;
    return cache[(size_t)n];
}
