/*
 * A180158  Number of ways are there to score a break of n points at snooker.
 *          Assuming an infinite number of reds are available, along with the
 *          usual six colors, and a break alternates red-color-red-...
 * https://oeis.org/A180158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-4) + a(n-5) + a(n-6) + a(n-7) + a(n-8) for n
 *            ≥ 8. Initial values: a(0)=1, a(1)=1, a(2)=0, a(3)=1, a(4)=2,
 *            a(5)=2, a(6)=3, a(7)=5 (OEIS A180158) The function keeps a
 *            static cache of already computed terms so that calls in any
 *            order are fast. It works for both plain integral `se...
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 64, u64 n <= 126,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Static cache for the sequence values. */
static seq_t *cache = NULL;     /* pointer to the array holding a(0)…a(k) */
static size_t cache_size = 0;   /* number of elements currently stored */

/* Initial values a(0) … a(7).  The array is accessed only when n < 8. */
static const seq_t base_vals[8] = {
    (seq_t)1,  /* a(0) */
    (seq_t)1,  /* a(1) */
    (seq_t)0,  /* a(2) */
    (seq_t)1,  /* a(3) */
    (seq_t)2,  /* a(4) */
    (seq_t)2,  /* a(5) */
    (seq_t)3,  /* a(6) */
    (seq_t)5   /* a(7) */
};

/* Ensure that the cache contains values up to index n (inclusive).  If it
   already does, do nothing.  Otherwise grow the cache, compute the new
   terms, and store them. */
static void ensure_cache(idx_t n)
{
    if (cache_size > (size_t)n)
        return;                      /* cache already large enough */

    size_t new_size = (size_t)n + 1;   /* needed size */
    seq_t *new_cache = NULL;
    size_t old_size = cache_size;

#ifdef __cplusplus
    /* Allocate new array with C++ new[] so that constructors are invoked. */
    new_cache = new seq_t[new_size];
    /* Copy existing data to the new array. */
    for (size_t i = 0; i < old_size; ++i)
        new_cache[i] = cache[i];
    /* Release the old array. */
    delete [] cache;
#else
    /* Allocate with C malloc.  The pointer will later be cast to seq_t*. */
    new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Out of memory – terminate the program (unlikely for n≈10000). */
        exit(1);
    }
    for (size_t i = 0; i < old_size; ++i)
        new_cache[i] = cache[i];
    free(cache);
#endif

    cache   = new_cache;
    cache_size = new_size;

    /* Fill the new entries.  For indices < 8 use the pre‑defined values. */
    for (size_t i = old_size; i < cache_size; ++i) {
        seq_t val;
        if (i < 8) {
            val = base_vals[(size_t)i];
        } else {
            /* Recurrence: sum of a(i‑3) … a(i‑8).  All indices are < i. */
            val =  cache[i-3] + cache[i-4] + cache[i-5]
                + cache[i-6] + cache[i-7] + cache[i-8];
        }
        cache[i] = val;
    }
}

/* Public sequence function.  It may be called in any order and for any
   value of n ≥ 0.  The function guarantees to return the correct value
   for all such n. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
