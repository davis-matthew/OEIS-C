/*
 * A015585  a(n) = 9*a(n-1) + 10*a(n-2).
 * https://oeis.org/A015585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 10*a(n-2) a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build-provided types:
   - seq_t: unsigned integer type or arbitrary-precision class
   - idx_t: unsigned integer type (uint64_t) */

/* This function memoises the sequence so that repeated calls
   are fast.  It uses simple dynamic allocation that works
   for both the fixed-size integer builds (C11) and the
   arbitrary-precision builds (C++17). */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* array of computed terms */
    static size_t  cache_cap = 0;       /* current allocated capacity */
    static idx_t   cache_len = 0;       /* number of terms stored (cache_len == 0 means none) */

    /* Ensure we have room for up to n */
    if (cache_len <= n) {
        size_t need = (size_t)n + 1;          /* required capacity including index n */
        if (need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap : 4;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;        /* allocation failure fallback */
            cache    = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)            cache[0] = (seq_t)0;
            else if (i == 1)       cache[1] = (seq_t)1;
            else                   cache[i] = (seq_t)9 * cache[i-1] + (seq_t)10 * cache[i-2];
        }
        cache_len = n + 1;         /* we now have values 0 .. n */
    }

    return cache[n];
}
