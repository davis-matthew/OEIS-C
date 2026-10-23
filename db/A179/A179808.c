/*
 * A179808  Number of n X 3 arrays with every diagonal and antidiagonal of
 *          length L containing a permutation of 1..L.
 * https://oeis.org/A179808
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) + 2*a(n-4) - 4*a(n-5)
 *            Initial values: a(3)=0, a(4)=1, a(5)=4, a(6)=9, a(7)=12, a(8)=25
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[i] holds a(i) for i >= 0 */
static size_t cache_cap = 0;    /* allocated length of cache */
static idx_t  cache_last = 0;   /* largest index that has been computed */

static void ensure_cache(idx_t n)
{
    /* Ensure cache has room for indices up to n */
    if ((size_t)(n + 1) <= cache_cap) return;
    size_t newcap = (size_t)(n + 1);
    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newptr) abort();      /* allocation failure: terminate */
    cache = newptr;
    cache_cap = newcap;
}

static void init_cache(void)
{
    /* Allocate space for indices up to 7 and set known values */
    ensure_cache(7);
    for (idx_t i = 0; i < 3; ++i) cache[i] = 0; /* unused below offset */
    cache[3] = 0;
    cache[4] = 1;
    cache[5] = 4;
    cache[6] = 9;
    cache[7] = 12;
    cache_last = 7;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* behaviour below offset is unspecified */

    if (cache == NULL) init_cache();

    if (n > cache_last) {
        ensure_cache(n);
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            /* recurrence: a(i) = 2*a(i-1) - a(i-2) + 2*a(i-3) + 2*a(i-4) - 4*a(i-5) */
            cache[i] =
                2 * cache[i-1] - cache[i-2] + 2 * cache[i-3] + 2 * cache[i-4] - 4 * cache[i-5];
        }
        cache_last = n;
    }

    return cache[n];
}
