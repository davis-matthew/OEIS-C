/*
 * A014021  Inverse of 12th cyclotomic polynomial.
 * https://oeis.org/A014021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-2) - a(n-4), with a(0)=1 and a(k)=0 for k<0. This
 *            recurrence follows from the generating function 1/(1−x²+x⁴)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation, valid for all calls */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* number of computed elements   */
    static size_t cache_cap  = 0;      /* capacity of allocated array   */

    /* Ensure we have enough capacity */
    if ((size_t)n >= cache_cap) {
        /* grow to at least n+1 elements; use a minimal growth policy */
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 16) new_cap = 16;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing entries using the recurrence */
    if ((size_t)n >= cache_size) {
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            } else {
                val = 0;
                if (i >= 2) val += cache[i - 2];   /* a(i-2), zero if i-2 < 0 */
                if (i >= 4) val -= cache[i - 4];   /* a(i-4), zero if i-4 < 0 */
            }
            cache[i] = val;
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
