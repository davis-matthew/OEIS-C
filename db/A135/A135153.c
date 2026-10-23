/*
 * A135153  Repeat Pell numbers A000129.
 * https://oeis.org/A135153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-4), with initial values a(0)=a(1)=0,
 *            a(2)=a(3)=1. Uses a simple cache that grows lazily
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 53, u64 n <= 103,
 *            u128 n <= 203, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
#   include <new>
#endif

/* static cache state */
static seq_t *cache  = NULL;   /* array of computed values     */
static idx_t  cap    = 0;      /* number of slots allocated    */
static idx_t  upto   = 0;      /* highest index for which cache[i] is valid */

/* ensure that cache holds indices 0 .. n (inclusive) */
static void
ensure_cache_size(idx_t n)
{
    if (cache != NULL && cap > n) return;          /* already large enough */

    /* first allocation */
    if (cache == NULL) {
        /* make room for at least indices 0..3 */
        size_t new_cap = n + 1;
        if (new_cap < 4) new_cap = 4;
        /* allocate */
#if defined(__cplusplus)
        seq_t *tmp = new (std::nothrow) seq_t[new_cap];
#else
        seq_t *tmp = (seq_t*)calloc(new_cap, sizeof(seq_t));
#endif
        if (tmp == NULL) return;   /* allocation failed – caller will see 0 */

        /* set the base values */
        tmp[0] = (seq_t)0;
        tmp[1] = (seq_t)0;
        tmp[2] = (seq_t)1;
        tmp[3] = (seq_t)1;

        cache = tmp;
        cap   = new_cap;
        upto  = 3;
        return;
    }

    /* resize */
    size_t new_cap = n + 1;
    if (new_cap < 4) new_cap = 4;
    if (new_cap <= cap) return;   /* size already sufficient */

#if defined(__cplusplus)
    seq_t *tmp = new (std::nothrow) seq_t[new_cap];
    if (tmp == NULL) return;      /* allocation failed */
    /* copy existing values */
    for (idx_t i = 0; i < cap; ++i) tmp[i] = cache[i];
    /* free old storage */
    delete [] cache;
#else
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) return;      /* allocation failed */
    /* realloc preserves the existing content */
    cache = tmp;  /* will be updated after assignment */
#endif
    cache = tmp;
    cap   = new_cap;
    /* values beyond the previous cap are uninitialized;
       they will be filled by the recurrence before use. */
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* make sure the cache can hold the requested index */
    ensure_cache_size(n);
    if (cache == NULL) return (seq_t)0;   /* allocation failed */

    /* If we already computed this index, return it */
    if (n <= upto) return cache[n];

    /* compute missing values up to n */
    for (idx_t i = upto + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i - 2] + cache[i - 4];
    }
    upto = n;
    return cache[n];
}
