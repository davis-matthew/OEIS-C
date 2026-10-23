/*
 * A122554  Let S(1) = {1} and, for n>1 let S(n) be the smallest set
 *          containing x, 2x and x+2 for each element x in S(n-1). a(n) is the
 *          number of elements in S(n).
 * https://oeis.org/A122554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4) for n > 6, a(1)=1,
 *            a(2)=3, a(3)=6, a(4)=10, a(5)=15, a(6)=23
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation --------------------------------------- */
static seq_t *cache = NULL;        /* array indexed by n (1‑based)          */
static idx_t  cache_cap = 0;        /* allocated capacity of cache          */
static idx_t  cache_max = 0;        /* largest n for which a(n) is stored   */

/* Grow the cache array to accommodate indices up to 'n'.              */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap)    /* already large enough */
        return;

    /* Determine new capacity: double each time, but at least 16.    */
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *new_cache = NULL;

#ifdef __cplusplus
    /* In C++ allocate with 'new' so constructors are called.       */
    new_cache = new seq_t[new_cap];
    /* Copy existing values. */
    for (idx_t i = 1; i <= cache_max && i < new_cap; ++i)
        new_cache[i] = cache[i];
    /* Free old storage. */
    if (cache)
        delete [] cache;
#else
    /* In C allocate with realloc (no constructors).                */
    new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif

    if (!new_cache)
        abort();                 /* out‑of‑memory */

    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute sequence values up to and including index 'n'.              */
static void compute_up_to(idx_t n)
{
    if (n <= cache_max)
        return;                  /* already computed */

    ensure_capacity(n);

    /* Initialise first six terms if not yet present. */
    if (cache_max < 6) {
        static const seq_t init[7] = { 0, 1, 3, 6, 10, 15, 23 };
        idx_t upto = n < 6 ? n : 6;
        for (idx_t i = cache_max + 1; i <= upto; ++i)
            cache[i] = init[i];
        cache_max = upto;
    }

    /* Recurrence for n > 6. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = 3 * cache[i - 1]
                 - 2 * cache[i - 2]
                 -   cache[i - 3]
                 +   cache[i - 4];
    }

    cache_max = n;
}

/* Public entry point ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* sequence defined for n >= 1 */

    compute_up_to(n);
    return cache[n];
}
