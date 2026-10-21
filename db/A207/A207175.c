/*
 * A207175  Number of 7 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 25*a(n-3) + 6*a(n-4) + 6*a(n-5) - 108*a(n-6) -
 *            72*a(n-7) + 216*a(n-9) for n > 11 a(1) = 14, a(2) = 196, a(3) =
 *            546, a(4) = 3141, a(5) = 11284, a(6) = 26866, a(7) = 98224, a(8)
 *            = 357356, a(9) = 1032000, a(10) = 3365824, a(11) = 11680176
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 34, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: all arithmetic is performed with unsigned seq_t values. The
   recurrence never causes intermediate underflow for the ranges
   considered here. */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] = a(1), cache[1] = a(2), ...  */
static size_t cache_cap = 0;     /* allocated capacity of cache          */
static size_t cache_size = 0;    /* number of elements currently stored  */

/* Initial 11 terms of the sequence */
static const seq_t init_vals[11] = {
    (seq_t)14ULL,
    (seq_t)196ULL,
    (seq_t)546ULL,
    (seq_t)3141ULL,
    (seq_t)11284ULL,
    (seq_t)26866ULL,
    (seq_t)98224ULL,
    (seq_t)357356ULL,
    (seq_t)1032000ULL,
    (seq_t)3365824ULL,
    (seq_t)11680176ULL
};

/* Ensure cache has room for at least 'needed' elements.  */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) abort();
    if (cache) {
        memcpy(tmp, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache     = tmp;
    cache_cap = new_cap;
}

/* Main function to evaluate the sequence. */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1; values for n=0 are not defined. */
    if (n == 0) return (seq_t)0;

    /* Small indices are fetched directly from the constant table. */
    if (n <= 11) return init_vals[n - 1];

    /* Ensure we have a cache that holds at least the first 11 terms. */
    if (cache == NULL) {
        grow_cache(11);
        for (size_t i = 0; i < 11; ++i) {
            cache[i] = init_vals[i];
        }
        cache_size = 11;
    }

    /* Extend the cache up to n if necessary. */
    if (cache_size < (size_t)n) {
        if ((size_t)n > cache_cap) {
            grow_cache((size_t)n);
        }
        /* Compute terms from a(12) onward. */
        for (size_t idx = cache_size; idx < (size_t)n; ++idx) {
            /* idx is the 0‑based index of a(idx+1).  For idx >= 11
               (i.e., a(n) with n >= 12) all required references are
               within the cache. */
            seq_t val = cache[idx - 1];                 /* a(n-1)   */
            val += (seq_t)25 * cache[idx - 3];           /* 25*a(n-3) */
            val += (seq_t)6  * cache[idx - 4];           /* 6*a(n-4)  */
            val += (seq_t)6  * cache[idx - 5];           /* 6*a(n-5)  */
            val -= (seq_t)108 * cache[idx - 6];          /* -108*a(n-6) */
            val -= (seq_t)72  * cache[idx - 7];          /* -72*a(n-7)  */
            val += (seq_t)216 * cache[idx - 9];          /* +216*a(n-9) */
            cache[idx] = val;
        }
        cache_size = (size_t)n;
    }

    return cache[n - 1];
}
