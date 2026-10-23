/*
 * A021048  Expansion of 1/((1-x)*(1-2x)*(1-3x)*(1-9x)).
 * https://oeis.org/A021048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=15, a(2)=160, a(3)=1530; for n>=4: a(n) = 15*a(n-1)
 *            - 65*a(n-2) + 105*a(n-3) - 54*a(n-4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
# include <utility>
#endif

/* ------------------------------------------------------------------ */
/* Static cache to memoise previously computed terms.  The cache is
   grown lazily so that the function remains efficient for any order
   of calls.  The cache is stored as an array of seq_t objects.  In C++
   the array elements are constructed by `new[]` and moved on grow.
   In C the array is reallocated with `realloc`.  This dual
   implementation works on both C11 and C++17. */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;          /* array of seq_t objects            */
static size_t cache_size = 0;          /* number of elements already set   */
static size_t cache_cap   = 0;          /* current capacity of the array    */

static void ensure_cache(idx_t n)
{
    /* Ensure that cache can hold indices 0..n inclusive. */
    size_t target = (size_t)n + 1;
    if (cache_cap < target) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < target) new_cap <<= 1;

#ifdef __cplusplus
        /* Allocate new array, default‑construct elements. */
        seq_t *tmp = new seq_t[new_cap];
        /* Move old values to the new array. */
        for (size_t i = 0; i < cache_size; ++i)
            tmp[i] = std::move(cache[i]);
        /* Delete old storage. */
        delete [] cache;
        cache = tmp;
#else
        /* C: realloc the old block; cast the returned void*
           pointer to seq_t*.  This works because seq_t is a
           trivially‑constructible unsigned integer type. */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failed */
        cache = tmp;
#endif
        cache_cap = new_cap;
    }
}

/* Helper to initialise the first four terms exactly once. */
static void init_initial_terms(void)
{
    if (cache_size == 0) {
        ensure_cache(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)15;
        cache[2] = (seq_t)160;
        cache[3] = (seq_t)1530;
        cache_size = 4;
    }
}

seq_t a(idx_t n)
{
    /* Fast return for the first four values without touching the cache. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)160;
    if (n == 3) return (seq_t)1530;

    init_initial_terms();
    ensure_cache(n);

    /* If the value is already cached, return it directly. */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Compute missing terms incrementally up to n. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t val = (seq_t)15 * cache[i-1]
                  - (seq_t)65 * cache[i-2]
                  + (seq_t)105 * cache[i-3]
                  - (seq_t)54 * cache[i-4];
        cache[i] = val;
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
