/*
 * A056236  a(n) = (2 + sqrt(2))^n + (2 - sqrt(2))^n.
 * https://oeis.org/A056236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056236:
 *  a(n) = (2 + sqrt(2))^n + (2 - sqrt(2))^n
 *  Recurrence:  a(0) = 2,  a(1) = 4,
 *                a(n) = 4*a(n-1) - 2*a(n-2)   for n ≥ 2
 */
#include <stddef.h>   /* for NULL, size_t */
#include <stdlib.h>   /* for malloc, free */
#if defined(__cplusplus)
#  include <new>     /* for operator new[] / delete[] with class seq_t */
#endif

/* Helper to allocate an array of seq_t objects */
static seq_t* allocate_seq(idx_t count)
{
#if defined(__cplusplus)
    return new seq_t[count];
#else
    return (seq_t*)malloc((size_t)count * sizeof(seq_t));
#endif
}

/* Helper to free an array of seq_t objects */
static void deallocate_seq(seq_t* ptr)
{
#if defined(__cplusplus)
    delete[] ptr;
#else
    free(ptr);
#endif
}

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t* cache   = NULL;
    static idx_t  cached  = 0;   /* number of values stored (indices 0..cached-1) */
    static idx_t  cap     = 0;   /* allocated size of cache array            */

    /* Initialise cache on first call */
    if (cache == NULL)
    {
        cap     = 2;
        cache   = allocate_seq(cap);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)4;
        cached  = 2;
    }

    /* Return value if already computed */
    if (n < cached)
        return cache[n];

    /* Grow cache if necessary */
    if (n >= cap)
    {
        idx_t new_cap = cap;
        while (new_cap <= n)
            new_cap <<= 1;              /* exponential growth */
        seq_t* new_cache = allocate_seq(new_cap);
        for (idx_t i = 0; i < cached; ++i)
            new_cache[i] = cache[i];
        deallocate_seq(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute remaining terms up to n using the recurrence */
    for (idx_t i = cached; i <= n; ++i)
    {
        seq_t term = cache[i-1] * ((seq_t)4);
        term      -= cache[i-2] * ((seq_t)2);
        cache[i]   = term;
    }

    cached = n + 1;          /* update number of computed values */
    return cache[n];
}
