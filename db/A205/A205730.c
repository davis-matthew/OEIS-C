/*
 * A205730  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock having
 *          the same number of clockwise edge increases as its horizontal
 *          neighbors and no 2 X 2 subblock having the same number of
 *          counterclockwise edge increases as its vertical neighbors.
 * https://oeis.org/A205730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) for n>14 satisfies a(n) = a(n-1) + 11*a(n-2) - 9*a(n-3) +
 *            8*a(n-4) - 26*a(n-5) - 99*a(n-6) + 51*a(n-7) + 135*a(n-8) +
 *            25*a(n-9) - 54*a(n-10) - 42*a(n-11) with initial values (a(1) …
 *            a(14)): 369, 978, 3318, 9198, 34674, 97380, 370008, 1053624,
 *            4060848, 11593452, 44691564, 127862298, 493245942...
 * Range:     u8 none, u16 n <= 5, u32 n <= 14, u64 n <= 33, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[15] = {
    (seq_t)0,         /* index 0 unused */
    (seq_t)369,       /* a(1) */
    (seq_t)978,       /* a(2) */
    (seq_t)3318,      /* a(3) */
    (seq_t)9198,      /* a(4) */
    (seq_t)34674,     /* a(5) */
    (seq_t)97380,     /* a(6) */
    (seq_t)370008,    /* a(7) */
    (seq_t)1053624,   /* a(8) */
    (seq_t)4060848,   /* a(9) */
    (seq_t)11593452,  /* a(10) */
    (seq_t)44691564,  /* a(11) */
    (seq_t)127862298, /* a(12) */
    (seq_t)493245942, /* a(13) */
    (seq_t)1412611194/* a(14) */
};

seq_t a(idx_t n)
{
    /* If within the known initial range, return directly. */
    if (n <= 14) {
        return init[n];
    }

    static seq_t *cache = NULL;     /* 1‑based cache of values a(1)… */
    static idx_t cache_cap = 0;     /* allocated capacity (in elements) */
    static idx_t cache_len = 0;     /* currently computed length */

    /* First allocation (if needed) */
    if (cache_len == 0) {
        cache_cap = 16;                               /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure: return zero */

        /* copy the initial 14 values */
        for (idx_t i = 1; i <= 14; ++i) {
            cache[i] = init[i];
        }
        cache_len = 14;
    }

    /* Grow the cache to fit up to n elements if necessary. */
    if (n + 1 > cache_cap) {            /* need room for index n */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values from cache_len+1 up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* The recurrence uses only additions and subtractions.
           All positive terms are accumulated first, then subtractions,
           ensuring the intermediate sum never underflows.   */
        seq_t sum = cache[i - 1];
        sum += (seq_t)11 * cache[i - 2];
        sum += (seq_t)8  * cache[i - 4];
        sum += (seq_t)51 * cache[i - 7];
        sum += (seq_t)135* cache[i - 8];
        sum += (seq_t)25 * cache[i - 9];

        sum -= (seq_t)9  * cache[i - 3];
        sum -= (seq_t)26 * cache[i - 5];
        sum -= (seq_t)99 * cache[i - 6];
        sum -= (seq_t)54 * cache[i -10];
        sum -= (seq_t)42 * cache[i -11];

        cache[i] = sum;
    }
    cache_len = n;                      /* last computed index */

    return cache[n];
}
