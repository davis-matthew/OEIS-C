/*
 * A087842  a(n) = a(a(a(n - a(n-1)))) + a(n - a(n-2)).
 * https://oeis.org/A087842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(a(n - a(n-1)))) + a(n - a(n-2)) with initial terms
 *            a(1)=1, a(2)=1, a(3)=2 (indices are 1‑based; a(0) is unused but
 *            defined as 0 for convenience)
 * Range:     u8 n <= 2904, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array of memoised terms */
static size_t  cache_cap = 0;     /* allocated elements in cache */
static size_t  cache_nmax = 0;    /* largest index that has been computed */

/*--- grow the cache so that it can hold index n (inclusive) ----------------*/
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;                 /* indices 0 … n   */
    if (cache_cap >= need) return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                          /* out of memory */

    cache = tmp;
    /* initialise new cells to zero – sequence values are built gradually */
    for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
    cache_cap = new_cap;
}

/*--- public function --------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined for n<1 */

    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        /* zero‑initialise the array */
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        /* base values: a(0)=0, a(1)=1, a(2)=1, a(3)=2 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache_nmax = 3;            /* highest computed index so far */
    }

    /* for indices ≤3 the value is already known */
    if (n <= cache_nmax)
        return cache[(size_t)n];

    /* make room for required indices */
    ensure_cache(n);

    /* compute missing terms from cache_nmax+1 up to n inclusive */
    for (size_t idx = cache_nmax + 1; idx <= (size_t)n; ++idx) {
        /* a(idx-1) and a(idx-2) are available */
        idx_t a_prev1 = (idx_t)cache[idx - 1];
        idx_t a_prev2 = (idx_t)cache[idx - 2];

        /* Term1: a(a(a(idx - a(idx-1)))) */
        idx_t t1 = idx - a_prev1;           /* idx - a(idx-1)          */
        seq_t v1 = cache[(size_t)t1];       /* a(t1)                    */
        idx_t t2 = (idx_t)v1;               /* argument for next a()   */
        seq_t v2 = cache[(size_t)t2];       /* a(t2)                    */
        idx_t t3 = (idx_t)v2;               /* argument for final a()  */
        seq_t v3 = cache[(size_t)t3];       /* a(t3)                    */

        /* Term2: a(idx - a(idx-2)) */
        idx_t t4 = idx - a_prev2;           /* idx - a(idx-2)          */
        seq_t v4 = cache[(size_t)t4];       /* a(t4)                    */

        cache[idx] = v3 + v4;
    }

    cache_nmax = (size_t)n;                 /* update highest computed */
    return cache[(size_t)n];
}
