/*
 * A178869  a(n) = 9*a(n-1) - 10*a(n-2); a(0)=0, a(1)=1.
 * https://oeis.org/A178869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 9*a(n-1) - 10*a(n-2); a(0)=0, a(1)=1
 *
 * Computes the nth term of OEIS sequence A178869 using memoisation.
 * The implementation works for both C and C++ builds where seq_t may be
 * an unsigned integer type of 8, 16, 32, 64 or 128 bits, or an
 * arbitrary‑precision class.  It uses malloc/realloc in C and
 * new/delete[] in C++ for dynamic allocation of the cache.
 */
#include <stdlib.h>   /* malloc, realloc */
#ifdef __cplusplus
    /* C++ compilers have a built‑in new/delete */
#endif

seq_t a(idx_t n)
{
    /* Static cache for memoisation – external linkage only, no “static” qualifier. */
    static seq_t *cache = NULL;       /* array of computed values */
    static idx_t cache_cap = 0;       /* allocated capacity (number of slots) */
    static idx_t cache_max = (idx_t)-1; /* highest index stored (valid values up to cache_max) */

    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Initialise the cache if it does not exist yet. */
    if (cache == NULL) {
        cache_cap = 2;
#if defined(__cplusplus)
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#endif
        if (cache == NULL) return (seq_t)0;      /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_max = 1;
    }

    /* Grow the cache if required to hold index n. */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;   /* ensure capacity for indices 0 .. n inclusive */
        seq_t *new_ptr;
#if defined(__cplusplus)
        /* For C++ use new/delete[]. */
        new_ptr = new seq_t[new_cap];
        /* Copy existing values. */
        for (idx_t i = 0; i < cache_cap; ++i) new_ptr[i] = cache[i];
        delete [] cache;
#else
        /* For C use realloc. */
        new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (new_ptr == NULL) return (seq_t)0;   /* allocation failed */
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val = (seq_t)9 * cache[i - 1] - (seq_t)10 * cache[i - 2];
        cache[i] = val;
    }
    cache_max = n;

    return cache[n];
}
