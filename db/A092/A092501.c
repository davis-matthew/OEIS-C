/*
 * A092501  a(0)=0, a(1)=1, a(2)=10; for n>2, a(n) = a(n-1)^2 + 10.
 * https://oeis.org/A092501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=10, a(n)=a(n-1)^2 + 10 for n>2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include only headers that are valid for both C and C++ builds. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* -----------------------  sequence implementation  ----------------------- */

seq_t a(idx_t n)
{
    /* Cache the already computed terms.  It grows on demand and permits
       out‑of‑order accesses. */
    static seq_t *cache          = NULL;   /* array of terms */
    static size_t cache_size    = 0;       /* allocated length */
    static idx_t  max_computed  = (idx_t)-1;  /* highest index with a value */
    static bool    base_initialized = false;

    /* Initialise the cache with the first three terms if necessary. */
    if (!base_initialized)
    {
        /* Ensure space for at least three entries. */
        size_t need = 3;
        if (cache_size < need)
        {
            seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* allocation failed */
            /* initialise new entries to zero */
            for (size_t i = cache_size; i < need; ++i)
                tmp[i] = (seq_t)0;
            cache = tmp;
            cache_size = need;
        }

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)10;
        max_computed = 2;
        base_initialized = true;
    }

    /* If the requested index is already computed, return it. */
    if (n <= max_computed)
        return cache[n];

    /* Grow the cache if the requested index is beyond the current capacity. */
    size_t new_limit = (size_t)n + 1;
    if (new_limit > cache_size)
    {
        size_t new_size = new_limit * 2;   /* simple growth strategy */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;         /* allocation failed */
        /* initialise the freshly allocated area */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n, using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t prev;
        prev = cache[i - 1];

        /* Square the previous term.  For C builds the intermediate
           multiplication uses unsigned __int128 to avoid overflow
           before casting back.  For C++ (big–int build) the class
           operator* handles the operation. */
#ifdef __cplusplus
        seq_t sq = prev * prev;
#else
        seq_t sq = (seq_t)((unsigned __int128)prev * prev);
#endif
        cache[i] = sq + (seq_t)10;
    }
    max_computed = n;
    return cache[n];
}
