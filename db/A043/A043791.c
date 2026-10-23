/*
 * A043791  Numbers whose number of runs in base 3 is congruent to 0 (mod 7).
 * https://oeis.org/A043791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A043791:  Numbers whose number of runs
 * in base‑3 is congruent to 0 (mod 7).
 *
 * a(n) is the n‑th positive integer whose base‑3 representation
 * contains a number of runs that is a multiple of 7.
 * The code incrementally searches for such numbers, caching
 * found terms.  The cache is implemented with a dynamic array,
 * which is allocated via C++ new[] when compiling as C++ and
 * via malloc/realloc when compiling as C.  This makes the
 * implementation work for both ordinary integer seq_t types
 * and arbitrary‑precision integer classes.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper: count the number of runs in the base‑3 representation of
 * a seq_t value.  For zero the representation is “0” which has
 * exactly one run. */
static inline size_t runs_in_base3(seq_t v)
{
    if (v == (seq_t)0)
        return 1;

    size_t runs = 0;
    bool first = true;
    seq_t prev = 0;
    while (v != (seq_t)0) {
        seq_t d = v % (seq_t)3;
        if (first || d != prev) {
            ++runs;
            prev = d;
            first = false;
        }
        v /= (seq_t)3;
    }
    return runs;
}

/* Static cache for discovered terms.  The array is allocated
 * with C++ new[] when compiling as C++ and with malloc/realloc
 * when compiling as C. */
static seq_t *cache      = NULL;   /* array of found terms */
static size_t cache_len  = 0;      /* number of terms currently stored */
static size_t cache_cap  = 0;      /* allocated size of the array */
static idx_t  remain_from = 0;     /* next integer (uint64_t) to test */

/* Grow the cache to hold at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need)
        new_cap <<= 1;
#ifdef __cplusplus
    /* C++: use new[] and delete[] */
    seq_t *tmp = new seq_t[new_cap];
    if (cache) {
        /* move or copy existing elements */
        for (size_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];
        delete[] cache;
    }
#else
    /* C: use malloc/realloc */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed: terminate */
        exit(1);
    }
#endif
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function required by the build system.  It returns the
 * n‑th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Check the current integer for the run property. */
        if (runs_in_base3((seq_t)remain_from) % 7 == 0) {
            if (cache_len == cache_cap)
                grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)remain_from;
        }
        ++remain_from;
    }
    /* idx_t may be larger than size_t, so cast carefully. */
    return cache[(size_t)(n - 1)];
}
