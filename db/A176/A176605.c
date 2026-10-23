/*
 * A176605  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=1, k=0 and l=1.
 * https://oeis.org/A176605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A176605 – sequence defined by a(0)=1, a(1)=1 and
    a(n+1)=∑_{p=0}^{n} a(p)*a(n-p)+1  (n≥1)
    (k=0, l=1).  Using the equivalent linear recurrence
    (n+1)*a(n)+2*(-3n+1)*a(n-1)+(9n-13)*a(n-2)+8*(-n+3)*a(n-3)+4*(n-4)*a(n-4)=0
    for n≥4.  Initial values:
    a(0)=1, a(1)=1, a(2)=3, a(3)=8.  The function memoises the values
    so that multiple calls with any order of indices are fast.  All
    arithmetic is performed in the type seq_t supplied by the build.   */

#include <stddef.h>
#include <stdlib.h>

/*   the build defines:
        typedef ... idx_t;      // unsigned 64‑bit integer
        typedef ... seq_t;      // unsigned integer or big‑int type   */

extern seq_t a ();            /* declaration provided elsewhere */

seq_t a(idx_t n)
{
    /* static cache for computed values           */
    static seq_t *cache = NULL;         /* array a[0 … last]          */
    static size_t cache_size = 0;       /* number of elements reserved */
    static idx_t last_cached = (idx_t)-1;/* highest computed index      */

    /* initialise cache with the first four terms */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) abort();     /* out‑of‑memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)8;
        last_cached = 3;
    }

    /* return if already computed */
    if (n < cache_size)
        return cache[n];

    /* grow the cache to hold a[0 … n] */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) abort();          /* out‑of‑memory */
    cache = tmp;
    cache_size = new_size;

    /* compute missing terms using the linear recurrence */
    for (idx_t i = last_cached + 1; i <= n; ++i) {
        if (i <= 3)           /* just in case */
            continue;

        /* coefficients: 2*(3i-1) = 6i-2,  9i-13,
                         8*(i-3),          4*(i-4)  */
        seq_t t1 = (seq_t)(6 * i - 2) * cache[i - 1];
        seq_t t2 = (seq_t)(9 * i - 13) * cache[i - 2];
        seq_t t3 = (seq_t)(8 * (i - 3)) * cache[i - 3];
        seq_t t4 = (seq_t)(4 * (i - 4)) * cache[i - 4];

        seq_t pos = t1 + t3; /* terms with positive sign   */
        seq_t neg = t2 + t4; /* terms with negative sign  */
        seq_t numerator = pos - neg;   /* pos ≥ neg for sequence terms */

        cache[i] = numerator / (i + 1);
    }
    last_cached = n;
    return cache[n];
}
