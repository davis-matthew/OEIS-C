/*
 * A223243  3-loop graph coloring a rectangular array: number of n X 4 0..6
 *          arrays where 0..6 label nodes of a graph with edges (0,1), (1,2),
 *          (2,0), (0,3), (3,4), (4,0), (0,5), (5,6), and (6,0), and every
 *          array movement to a horizontal or vertical neighbor moves along an
 *          edge of this graph.
 * https://oeis.org/A223243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 40*a(n-1) - 80*a(n-2) - 869*a(n-3) + 1566*a(n-4) +
 *            1650*a(n-5) - 1796*a(n-6) for n>7 a(1)=168, a(2)=4086,
 *            a(3)=144582, a(4)=5336844, a(5)=198634758, a(6)=7399451382,
 *            a(7)=275682413748
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* External types provided by the build environment */
extern seq_t a(idx_t n);   /* prototype for the compiler */

static seq_t *cache = NULL;          /* 1‑based indexed cache of computed terms */
static idx_t  cache_capacity = 0;    /* allocated size (n+1) */
static idx_t  cached_max = 0;        /* highest n that has been computed */

static const seq_t base_values[8] = {
    0,                      /* unused index 0 */
    168ULL,                 /* a(1) */
    4086ULL,                /* a(2) */
    144582ULL,              /* a(3) */
    5336844ULL,             /* a(4) */
    198634758ULL,           /* a(5) */
    7399451382ULL,          /* a(6) */
    275682413748ULL         /* a(7) */
};

static void resize_cache(idx_t new_cap)
{
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (!new_cache) abort();

    if (cache) {
        memcpy(new_cache, cache, sizeof(seq_t) * cache_capacity);
        free(cache);
    }
    cache = new_cache;
    cache_capacity = new_cap;

    if (new_cap > 7) {
        for (idx_t i = 1; i <= 7; ++i) cache[i] = base_values[i];
    }
    if (cached_max < 7) cached_max = 7;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n<1 */

    /* Allocate / grow the cache if necessary */
    if (!cache || cache_capacity <= n) {
        idx_t new_cap = cache_capacity ? cache_capacity : 8;
        while (new_cap <= n) new_cap <<= 1;
        resize_cache(new_cap);
    }

    if (n <= 7) return cache[n];

    /* Compute values from cached_max+1 up to n using the recurrence */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        /* Use the recurrence: positive terms first, then subtract negative terms */
        seq_t sum = 0;
        sum += cache[k - 1] * (seq_t)40ULL;     /* +40*a(k-1) */
        sum += cache[k - 4] * (seq_t)1566ULL;   /* +1566*a(k-4) */
        sum += cache[k - 5] * (seq_t)1650ULL;   /* +1650*a(k-5) */
        sum -= cache[k - 2] * (seq_t)80ULL;     /* -80*a(k-2) */
        sum -= cache[k - 3] * (seq_t)869ULL;    /* -869*a(k-3) */
        sum -= cache[k - 6] * (seq_t)1796ULL;   /* -1796*a(k-6) */
        cache[k] = sum;
    }
    cached_max = n;
    return cache[n];
}
