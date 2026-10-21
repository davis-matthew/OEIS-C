/*
 * A240285  Number of 2 X n 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or two plus the sum of the elements
 *          above it or zero plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-2) - 24*a(n-4) + 31*a(n-6) - 16*a(n-8) initial
 *            values: a(1)=2, a(2)=8, a(3)=19, a(4)=76, a(5)=181, a(6)=741,
 *            a(7)=1779, a(8)=7308
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[9] = {
    0ULL,                         /* dummy entry for a(0) */
    2ULL,                         /* a(1) */
    8ULL,                         /* a(2) */
    19ULL,                        /* a(3) */
    76ULL,                        /* a(4) */
    181ULL,                       /* a(5) */
    741ULL,                       /* a(6) */
    1779ULL,                      /* a(7) */
    7308ULL                       /* a(8) */
};

static seq_t *cache = NULL;          /* array holding computed values */
static idx_t   cache_last = 0;       /* largest n already computed */
static size_t  cache_cap  = 0;       /* allocated capacity of cache */

/* enlarge cache if necessary and compute missing terms */
static void ensure_cache(idx_t n)
{
    if (n <= cache_last) return;                 /* nothing to do */

    /* grow array to hold 0..n (size n+1) */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { free(cache); exit(1); }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base values up to index 8 */
    for (idx_t i = 1; i <= 8 && i < (idx_t)cache_cap; ++i)
        cache[i] = init_vals[i];

    /* compute missing terms in order */
    for (idx_t k = cache_last + 1; k <= n; ++k) {
        if (k <= 8) {
            cache[k] = init_vals[k];
        } else {
            seq_t part1 = cache[k - 2] * 12 + cache[k - 6] * 31;
            seq_t part2 = cache[k - 4] * 24 + cache[k - 8] * 16;
            cache[k] = part1 - part2;
        }
    }
    cache_last = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1, return 0 safely */
    ensure_cache(n);
    return cache[n];
}
