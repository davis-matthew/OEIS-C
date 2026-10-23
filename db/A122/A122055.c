/*
 * A122055  A Somos 9-type recurrence: a(n) = (3*a(n-1)*a(n-8) -
 *          a(n-4)*a(n-5))/a(n-9), with a(0)=...=a(8)=1.
 * https://oeis.org/A122055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*a(n-1)*a(n-8) - a(n-4)*a(n-5))/a(n-9) with
 *            a(0)=a(1)=…=a(8)=1
 * Range:     u8 n <= 13, u16 n <= 18, u32 n <= 23, u64 n <= 31, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function keeps a static cache so that multiple calls in any
    order are O(1) after the first computation up to the largest n
    requested.  The implementation uses 128‑bit arithmetic only when
    seq_t is a builtin unsigned integer type; otherwise it relies on
    the arithmetic operators defined for the arbitrary‑precision
    seq_t class. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the trivial initial values without allocating the cache. */
    if (n <= 8) {
        return (seq_t)1;
    }

    /* static cache for memoisation ------------------------------------- */
    static seq_t *cache      = NULL;   /* holds computed values a(k) */
    static idx_t max_computed = 0;     /* highest index that has been computed */

    /* Allocate (and initialise) the cache if this is the first call. */
    if (cache == NULL) {
        /* Allocate up to max(n+1,9) elements so that future calls do not
           need reallocation too often. */
        size_t cap = (size_t)(n + 1);
        if (cap < 9) cap = 9;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) {
            return 0;  /* Out of memory – abort silently. */
        }
        /* Initialise a(0)…a(8) = 1. */
        for (idx_t i = 0; i <= 8; ++i) {
            cache[i] = (seq_t)1;
        }
        max_computed = 8;
    }

    /* Expand the cache if a larger n is requested. */
    if ((size_t)n + 1 > max_computed + 1) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            return 0;  /* Out of memory – abort silently. */
        }
        cache = tmp;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Use a wider intermediate type only if seq_t is a builtin
           unsigned integer type; otherwise rely on class arithmetic. */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            unsigned __int128 t1 = (unsigned __int128)(uint64_t)3
                                   * (unsigned __int128)cache[i-1]
                                   * (unsigned __int128)cache[i-8];
            unsigned __int128 t2 = (unsigned __int128)cache[i-4]
                                   * (unsigned __int128)cache[i-5];
            unsigned __int128 num = t1 - t2;
            cache[i] = (seq_t)(num / cache[i-9]);   /* division is exact */
        } else {
            cache[i] = ((seq_t)3 * cache[i-1] * cache[i-8]
                        - cache[i-4] * cache[i-5]) / cache[i-9];
        }
    }
    max_computed = n;

    return cache[n];
}
