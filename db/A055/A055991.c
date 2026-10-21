/*
 * A055991  a(n) is its own 4th difference.
 * https://oeis.org/A055991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) (n ≥ 5)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__cplusplus)
#include <new>
#endif

/* Memoised computation of A055991.  Cache contains a(0)=1 (auxiliary)
   and the first four non‑zero terms. */
static seq_t *cache      = NULL;   /* cached terms */
static size_t cache_cap  = 0;      /* allocated size of cache */
static idx_t  cache_max  = 0;      /* maximum index computed so far */

/* Allocate memory for 'cap' seq_t objects.  In C++ the objects are
   value‑initialised via placement new; in C the allocation is fine
   because seq_t is a builtin type. */
static void alloc_cache(size_t cap)
{
    cache = (seq_t*)malloc(sizeof(seq_t) * cap);
    if (cache == NULL) return;
#if defined(__cplusplus)
    for (size_t i = 0; i < cap; ++i) new (&cache[i]) seq_t();
#endif
    cache_cap = cap;
}

/* Ensure that cache contains computed values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initialise cache with room for indices 0..4 (needed by the recurrence) */
        alloc_cache(5);
        if (cache == NULL) return;
        cache[0] = (seq_t)1;   /* auxiliary value */
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)5;   /* a(2) */
        cache[3] = (seq_t)19;  /* a(3) */
        cache[4] = (seq_t)69;  /* a(4) */
        cache_max = 4;
    }

    /* Resize cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return;
        cache = tmp;
#if defined(__cplusplus)
        for (size_t i = cache_cap; i < new_cap; ++i) new (&cache[i]) seq_t();
#endif
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    while (cache_max < n) {
        idx_t i = cache_max + 1;
        seq_t t = (seq_t)5 * cache[i - 1]
                - (seq_t)6 * cache[i - 2]
                + (seq_t)4 * cache[i - 3]
                -            cache[i - 4];
        cache[i] = t;
        cache_max = i;
    }
}

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1 */
    ensure_cache(n);
    return cache[n];
}
