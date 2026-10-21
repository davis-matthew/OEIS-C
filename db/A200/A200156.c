/*
 * A200156  Number of 0..n arrays x(0..4) of 5 elements with zero 4th
 *          difference.
 * https://oeis.org/A200156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A200156 – Number of 0..n arrays of 5 elements with zero 4th difference
 *
 *  Recurrence (offset 1):
 *      a(n) = a(n-1)+a(n-2)+a(n-4)-3*a(n-5)-a(n-6)
 *            +a(n-8)+3*a(n-9)-a(n-10)-a(n-12)-a(n-13)+a(n-14)
 *  Initial values:
 *      a(1)=2, a(2)=15, a(3)=40, a(4)=103, a(5)=202, a(6)=381,
 *      a(7)=636, a(8)=1033, a(9)=1550, a(10)=2287, a(11)=3212,
 *      a(12)=4451, a(13)=5946, a(14)=7869
 *
 *  The function computes a(n) for any n≥1, using memoisation and a
 *  dynamic cache that grows as necessary.  All intermediate calculations
 *  are performed in type `seq_t`, which may be an arbitrary-precision
 *  integer type, so the implementation is correct for every supported
 *  build configuration.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Base values for 1 ≤ n ≤ 14  (index 0 unused) */
static const seq_t base[15] = {
    0,  /* base[0] unused */
    2, 15, 40, 103, 202, 381, 636, 1033, 1550,
    2287, 3212, 4451, 5946, 7869
};

static seq_t *cache = NULL;          /* dynamic cache, index starts at 1 */
static size_t cache_size = 0;        /* size of cache array (including index 0) */
static size_t cache_max = 0;         /* largest index that has been computed */

/* Ensures that cache has room for indices up to 'need' (inclusive).
 * Fills newly allocated slots with 0. */
static void ensure_cache_size(size_t need) {
    if (cache == NULL) {
        /* First allocation – enough for indices 0 … need */
        cache = seq_alloc(need + 1);
        cache_size = need + 1;
        /* Initialise base values */
        for (size_t i = 1; i <= 14 && i <= need; ++i)
            cache[i] = base[i];
        /* Newly allocated slots (≥15) are already 0 */
        cache_max = (need >= 14 ? 14 : need);
    } else if (cache_size <= need) {
        seq_t *tmp = seq_realloc(cache, cache_size, need + 1);
        cache = tmp;
        cache_size = need + 1;
        /* Initialise the new slots to 0 */
        for (size_t i = cache_size; i <= need; ++i)
            cache[i] = 0;
        /* cache_max stays unchanged */
    }
}

/* Computes a(n) for n >= 1 */
seq_t a(idx_t n) {
    /* For n < 15, return the hardwired values */
    if ((idx_t)n < 15) {
        return base[(size_t)n];
    }

    /* Grow cache if necessary */
    ensure_cache_size((size_t)n);

    /* If we already have a(n), just return it */
    if ((size_t)n <= cache_max) {
        return cache[(size_t)n];
    }

    /* Compute terms from cache_max + 1 up to n */
    size_t start = (cache_max < 15 ? 15 : cache_max + 1);
    for (size_t i = start; i <= (size_t)n; ++i) {
        cache[i] =
            cache[i-1] + cache[i-2] + cache[i-4]
            - 3 * cache[i-5] - cache[i-6]
            + cache[i-8] + 3 * cache[i-9]
            - cache[i-10] - cache[i-12]
            - cache[i-13] + cache[i-14];
    }

    cache_max = (size_t)n;
    return cache[(size_t)n];
}
