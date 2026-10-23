/*
 * A125344  Number of base 7 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A125344: Number of base‑7 circular n‑digit numbers with adjacent
    digits differing by at most 4.
    The sequence satisfies the linear recurrence

          a(0)=1, a(1)=7, a(2)=43, a(3)=247, a(4)=1527, a(5)=9497
          a(n)=7·a(n‑1)–3·a(n‑2)–11·a(n‑3)+a(n‑4)+3·a(n‑5)   for n≥6
    (the recurrence follows from the given generating function).
    The implementation uses a static cache to answer successive calls
    in O(1) after the first computation of a given index.  It works
    with integral types (uint8_t … uint64_t) and with arbitrary‑precision
    sequence types (when compiled as C++17). */

#include <stdlib.h>

/* Forward declare helper to free the cache when the program ends.
   The static destructor is harmless for C and C++ and keeps the
   compiler happy. */
static void free_cache(void);

static seq_t *cache = NULL;          /* cached terms */
static idx_t  cache_cap = 0;         /* allocated size (number of slots) */
static idx_t  cache_len = 0;         /* number of terms that have been
                                       computed (must be ≤ cache_cap) */

static void free_cache(void)
{
    if (cache != NULL) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = NULL;
        cache_cap = 0;
        cache_len = 0;
    }
}

static void grow_capacity(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    /* Allocate new array */
    seq_t *tmp;
#ifdef __cplusplus
    tmp = new seq_t[new_cap];
#else
    tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    if (tmp == NULL) {
        /* Allocation failed – leave the old cache untouched */
        return;
    }

    /* Copy existing terms */
    for (idx_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];

    /* Free old array */
    if (cache != NULL) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = tmp;
    cache_cap = new_cap;
}

/* Helper: initialise the cache with the first six terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;               /* already initialised */

    /* Initially allocate a few slots; they will be grown as needed. */
    cache_cap = 8;
#ifdef __cplusplus
    cache = new seq_t[cache_cap];
#else
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
    if (cache == NULL)
        return;               /* allocation failed – caller will handle 0 */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)7;
    cache[2] = (seq_t)43;
    cache[3] = (seq_t)247;
    cache[4] = (seq_t)1527;
    cache[5] = (seq_t)9497;
    cache_len = 6;

    /* Register cleanup at program exit (for C++ dynamic allocations). */
#ifndef __cplusplus
    /* In C there is no atexit registration required here. */
#endif
}

seq_t a(idx_t n)
{
    /* Ensure the cache is ready. */
    init_cache();
    if (cache == NULL)
        return (seq_t)0;     /* allocation failed */

    /* If the requested index is already known, return it. */
    if (n < cache_len)
        return cache[n];

    /* Grow the capacity if necessary. */
    grow_capacity(n + 1);
    if (cache == NULL)
        return (seq_t)0;     /* allocation failed after grow */

    /* Compute any missing terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* The recurrence applies for i ≥ 6; cache_len is always ≥ 6 at
           this point because we init the first six terms. */
        cache[i] = (seq_t)7 * cache[i - 1]
                 - (seq_t)3 * cache[i - 2]
                 - (seq_t)11 * cache[i - 3]
                 +          cache[i - 4]
                 + (seq_t)3 * cache[i - 5];
    }
    cache_len = n + 1;

    return cache[n];
}
