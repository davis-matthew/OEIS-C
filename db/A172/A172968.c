/*
 * A172968  a(n) = 7*a(n-1) - a(n-2) for n>1, a(0)=1, a(1)=2.
 * https://oeis.org/A172968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(n)=7*a(n-1)-a(n-2) for n>1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                /* size_t, NULL */
#if defined(__cplusplus)
#  include <new>                   /* operator new[] */
#endif

/* static cache for memoisation */
static seq_t *cache = NULL;        /* array of terms */
static size_t  cache_cap = 0;      /* allocated capacity */
static idx_t   cache_len = 0;      /* number of valid terms in the cache */

/* ensure cache contains at least (n+1) slots */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache == NULL) {
        /* initial allocation: at least 2 slots for a(0),a(1) */
        size_t cap = (need > 2 ? need : 2);
#if defined(__cplusplus)
        cache = new (std::nothrow) seq_t[cap];
        if (!cache) return;                     /* allocation failed */
#else
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return;                     /* allocation failed */
#endif
        /* initialise known first two terms */
        cache[0] = 1;
        cache[1] = 2;
        cache_cap = cap;
        cache_len = 2;
        return;
    }

    if (need <= cache_cap) return;              /* already enough */

    /* reallocate to the required size (simple strategy: grow exactly to need) */
    size_t new_cap = need;
#if defined(__cplusplus)
    seq_t *tmp = new (std::nothrow) seq_t[new_cap];
    if (!tmp) return;                           /* allocation failed */
    for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
    delete[] cache;
#else
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                           /* allocation failed */
    /* realloc preserves contents, no need to copy */
    cache = tmp;
#endif
    cache = tmp;
    cache_cap = new_cap;
}

/* --- public entry point --- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cache_len) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)          cache[0] = 1;
        else if (i == 1)     cache[1] = 2;
        else                 cache[i] = (seq_t)7 * cache[i-1] - cache[i-2];
    }
    cache_len = n + 1;
    return cache[n];
}
