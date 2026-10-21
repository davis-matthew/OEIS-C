/*
 * A200194  Number of -n..n arrays x(0..4) of 5 elements with zero sum,
 *          adjacent elements differing by more than one, and elements
 *          alternately increasing and decreasing.
 * https://oeis.org/A200194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) - 2*a(n-5) + 2*a(n-6) + a(n-8) -
 *            2*a(n-10) + a(n-11); initial values: a(1)=0, a(2)=24, a(3)=144,
 *            a(4)=506, a(5)=1298, a(6)=2794, a(7)=5300, a(8)=9220,
 *            a(9)=14974, a(10)=23094, a(11)=34120
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed_up_to = 0;   /* largest n for which cache[n] is defined */

static const seq_t init_vals[12] = {
    0,      /* a(0) unused but set to 0 for safety */
    0,      /* a(1) */
    24,     /* a(2) */
    144,    /* a(3) */
    506,    /* a(4) */
    1298,   /* a(5) */
    2794,   /* a(6) */
    5300,   /* a(7) */
    9220,   /* a(8) */
    14974,  /* a(9) */
    23094,  /* a(10) */
    34120   /* a(11) */
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* outside the offset – not defined */

    /* Ensure the cache is large enough to hold index n */
    idx_t needed = n + 1;            /* indices 0..n inclusive */
    if (cache == NULL) {
        if (needed < 12) needed = 12;
        cache_size   = needed;
        cache        = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure */
        for (idx_t i = 0; i < cache_size; ++i) cache[i] = 0;
        for (idx_t i = 1; i <= 11; ++i) cache[i] = init_vals[i];
        computed_up_to = 11;
    } else if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)needed * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failure */
        cache = tmp;
        /* initialise the newly allocated slots */
        for (idx_t i = cache_size; i < needed; ++i) cache[i] = 0;
        cache_size = needed;
    }

    if (n <= computed_up_to) return cache[n];

    /* Compute new values using the empirical recurrence */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t v = cache[i-1] + cache[i-1];          /* 2*a(i-1) */
        v = v - cache[i-3];                         /* - a(i-3) */
        v = v - (cache[i-5] + cache[i-5]);          /* - 2*a(i-5) */
        v = v + (cache[i-6] + cache[i-6]);          /* + 2*a(i-6) */
        v = v + cache[i-8];                         /* + a(i-8) */
        v = v - (cache[i-10] + cache[i-10]);        /* - 2*a(i-10) */
        v = v + cache[i-11];                        /* + a(i-11) */
        cache[i] = v;
    }
    computed_up_to = n;
    return cache[n];
}
