/*
 * A126967  Expansion of e.g.f.: sqrt(1+4*x)/(1+2*x).
 * https://oeis.org/A126967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -[6*(n-1)*a(n-1) + 4*(n-1)*(2*n-3)*a(n-2)] for n ≥ 2 with
 *            a(0) = 1, a(1) = 0
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------
   The following static cache holds all previously computed terms.
   It grows dynamically to accommodate any requested index.
   For C++ builds we use operator new/delete; for C we use malloc/free.
   ------------------------------------------------------------------ */
static seq_t *cache          = NULL;   /* array of computed terms   */
static size_t cache_len      = 0;      /* number of stored terms    */
static size_t cache_capacity = 0;      /* allocated capacity        */

/* ------------------------------------------------------------------
   Grow the cache so that it can hold at least indices up to `n`.
   Newly allocated space is initialized by copying existing elements.
   ------------------------------------------------------------------ */
static void
ensure_cache(idx_t n)
{
    /* Nothing to do if we already have the term. */
    if ((size_t)n < cache_len) return;

    size_t needed = (size_t)n + 1;         /* inclusive */
    if (needed > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 32;
        while (new_cap < needed) new_cap <<= 1;

        seq_t *new_cache;
#if defined(__cplusplus)
        new_cache = new seq_t[new_cap];
#else
        new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        if (!new_cache) abort();          /* allocation failure */

        /* Copy existing values */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* Release old memory */
        if (cache) {
#if defined(__cplusplus)
            delete[] cache;
#else
            free(cache);
#endif
        }

        cache          = new_cache;
        cache_capacity = new_cap;
    }

    /* Compute all missing terms up to `n` (inclusive) */
    for (size_t i = cache_len; i < needed; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)0;
        } else {
            seq_t im1  = (seq_t)(i - 1);           /* (n-1)   */
            seq_t term1 = (seq_t)6 * im1 * cache[i - 1];
            seq_t im2  = (seq_t)(2 * i - 3);       /* (2n-3)  */
            seq_t term2 = (seq_t)4 * im1 * im2 * cache[i - 2];
            cache[i] = -term1 - term2;
        }
    }

    cache_len = needed;
}

/* ------------------------------------------------------------------
   Public sequence function requested by the exercise.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
