/*
 * A144392  Inverse binomial transform of A061037 (read as with offset 0).
 * https://oeis.org/A144392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 24, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A144392: inverse binomial transform of A061037.
   The first 12 terms are given explicitly; for n ≥ 12 the sequence
   satisfies the linear recurrence

      a(n) = -(12*a[n-1] + 66*a[n-2] + 220*a[n-3] + 492*a[n-4]
                 + 768*a[n-5] + 840*a[n-6] + 624*a[n-7] + 288*a[n-8]
                 + 64*a[n-9])

   The recurrence follows from the rational generating function

      x*(5 + 53x + 273x² + 878x³ + 1934x⁴ + 3042x⁵ + 3462x⁶ + 2814x⁷
        + 1554x⁸ + 518x⁹ + 74x¹⁰) /
      (1 + 2x)³ (1 + 2x + 2x²)³ .
*/

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of the function.  The types seq_t and idx_t are
   supplied by the build system. */
seq_t a(idx_t n)
{
    /*----------------------------------------------------
      Helper: dynamic cache of computed terms.
      The cache is grown only when required.
    ----------------------------------------------------*/
#if defined(__cplusplus)
    /* In C++ use 'new' / 'delete[]' to default‑construct the objects. */
    static seq_t *cache = nullptr;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* Initial 12 terms (offset 0).  These are stored in the cache
       after the first call. */
    static const seq_t init[12] = {
        (seq_t)0,
        (seq_t)5,
        (seq_t)-7,
        (seq_t)27,
        (seq_t)-84,
        (seq_t)240,
        (seq_t)-630,
        (seq_t)1554,
        (seq_t)-3702,
        (seq_t)8694,
        (seq_t)-20310,
        (seq_t)47190
    };

    /* Lazy initialization of the cache.  The cache capacity is
       at least the required index plus one. */
    if (cache == nullptr) {
        cache_cap = (size_t)((n < 11) ? 12 : n + 1);
        cache = new seq_t[cache_cap];
        for (size_t i = 0; i < 12; ++i) cache[i] = init[i];
        cache_size = 12;
    }

    /* Resize if the requested index is beyond current capacity. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to 'n' using the recurrence. */
    if (cache_size <= (size_t)n) {
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            seq_t val;
            if (i < 12) {
                val = init[(size_t)i];
            } else {
                val = -(
                    (seq_t)12 * cache[i - 1] +
                    (seq_t)66 * cache[i - 2] +
                    (seq_t)220 * cache[i - 3] +
                    (seq_t)492 * cache[i - 4] +
                    (seq_t)768 * cache[i - 5] +
                    (seq_t)840 * cache[i - 6] +
                    (seq_t)624 * cache[i - 7] +
                    (seq_t)288 * cache[i - 8] +
                    (seq_t)64  * cache[i - 9]
                );
            }
            cache[i] = val;
        }
        cache_size = (size_t)n + 1;
    }

    return cache[n];
#else
    /* In C the same logic but using malloc/realloc.  No constructors
       are required because seq_t is a plain arithmetic type. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* Initial values. */
    static const seq_t init[12] = {
        (seq_t)0,
        (seq_t)5,
        (seq_t)-7,
        (seq_t)27,
        (seq_t)-84,
        (seq_t)240,
        (seq_t)-630,
        (seq_t)1554,
        (seq_t)-3702,
        (seq_t)8694,
        (seq_t)-20310,
        (seq_t)47190
    };

    if (cache == NULL) {
        cache_cap = (size_t)((n < 11) ? 12 : n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();   /* fatal allocation failure */
        for (size_t i = 0; i < 12; ++i) cache[i] = init[i];
        cache_size = 12;
    }

    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    if (cache_size <= (size_t)n) {
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            seq_t val;
            if (i < 12) {
                val = init[(size_t)i];
            } else {
                val = -(
                    (seq_t)12 * cache[i - 1] +
                    (seq_t)66 * cache[i - 2] +
                    (seq_t)220 * cache[i - 3] +
                    (seq_t)492 * cache[i - 4] +
                    (seq_t)768 * cache[i - 5] +
                    (seq_t)840 * cache[i - 6] +
                    (seq_t)624 * cache[i - 7] +
                    (seq_t)288 * cache[i - 8] +
                    (seq_t)64  * cache[i - 9]
                );
            }
            cache[i] = val;
        }
        cache_size = (size_t)n + 1;
    }

    return cache[n];
#endif
}
