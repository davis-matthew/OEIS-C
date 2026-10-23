/*
 * A167916  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   free_old_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167916 (OEIS)
   Recurrence:  a(0) … a(16) are given below.
   For n >= 17:  a(n) = 10 * (a(n-1)+…+a(n-15)) - 55 * a(n-16). */

/* Standard headers for dynamic memory and integer types */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/free_old_cache.h"

/* ------------------------------------------------------------
   Internal allocation helpers (similar to the ones in the
   provided utilities).  They work in both C and C++.
   ------------------------------------------------------------ */
#if defined(__cplusplus)
static void *alloc_new_cache(size_t new_cap)
{
    return static_cast<void*>(new char[new_cap * sizeof(seq_t)]());
}

#else
static void *alloc_new_cache(size_t new_cap)
{
    return (void*)malloc(new_cap * sizeof(seq_t));
}

#endif

/* ------------------------------------------------------------
   Cache of computed terms.
   ------------------------------------------------------------ */
static seq_t  *cache          = NULL;   /* cache[0 .. cache_size-1]   */
static size_t  cache_capacity = 0;      /* allocated number of slots   */
static size_t  cache_size     = 0;      /* number of values stored    */

/* ------------------------------------------------------------
   Initial values for a(0) … a(16).
   ------------------------------------------------------------ */
static const seq_t init_vals[17] = {
    (seq_t)1ULL,
    (seq_t)12ULL,
    (seq_t)132ULL,
    (seq_t)1452ULL,
    (seq_t)15972ULL,
    (seq_t)175692ULL,
    (seq_t)1932612ULL,
    (seq_t)21258732ULL,
    (seq_t)233846052ULL,
    (seq_t)2572306572ULL,
    (seq_t)28295372292ULL,
    (seq_t)311249095212ULL,
    (seq_t)3423740047332ULL,
    (seq_t)37661140520652ULL,
    (seq_t)414272545727172ULL,
    (seq_t)4556998002998892ULL,
    (seq_t)50126978032987746ULL
};

/* ------------------------------------------------------------
   Ensure the cache can hold at least new_cap elements.
   ------------------------------------------------------------ */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_capacity) return;
    size_t new_cap2 = cache_capacity ? cache_capacity << 1 : 16;
    if (new_cap2 < new_cap) new_cap2 = new_cap;
    seq_t *new_cache = (seq_t*)alloc_new_cache(new_cap2);
    if (!new_cache) abort();

    /* Copy existing data */
    for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];

    if (cache) free_old_cache(cache);
    cache          = new_cache;
    cache_capacity = new_cap2;
}

/* ------------------------------------------------------------
   Compute a(n).  The function is memoised; calls may come in any
   order and with any n >= 0.
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Return small terms directly from the table if possible. */
    if (n <= 16) return init_vals[(size_t)n];

    /* Initialise cache with first 17 values if not already done. */
    if (cache_size == 0) {
        ensure_cache_capacity(17);
        for (size_t i = 0; i < 17; ++i) cache[i] = init_vals[i];
        cache_size = 17;
    }

    /* Extend cache up to n.  For i >= 17 we use the recurrence. */
    if (cache_size <= (size_t)n) {
        ensure_cache_capacity((size_t)n + 1);
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            /* Compute sum of a(i-1) .. a(i-15). */
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 15; ++j) {
                sum += cache[(size_t)(i - j)];
            }
            /* a(i) = 10 * sum - 55 * a(i-16) */
            cache[(size_t)i] =
                sum * (seq_t)10 - cache[(size_t)(i - 16)] * (seq_t)55;
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
