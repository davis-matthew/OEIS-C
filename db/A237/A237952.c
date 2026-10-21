/*
 * A237952  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the upper
 *          median plus the lower median plus the minimum of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence a(1) … a(12) =
 *            256,1280,6400,34560,186624,1022976,5607424,30916608,
 *            170459136,939196416,5174788096,28562620416 a(n) = 976*a(n-4) -
 *            46848*a(n-8) + 110592*a(n-12) for n ≥ 13 The function caches
 *            previously computed values so that calls in any order are O(1)
 *            after th...
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[12] = {
    (seq_t)256, (seq_t)1280, (seq_t)6400, (seq_t)34560,
    (seq_t)186624, (seq_t)1022976, (seq_t)5607424,
    (seq_t)30916608, (seq_t)170459136,
    (seq_t)939196416, (seq_t)5174788096, (seq_t)28562620416
};

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; n=0 is unused */
        return (seq_t)0;

    static seq_t *cache  = NULL;   /* pointer to cached terms a(1), a(2), … */
    static size_t cache_len = 0;    /* number of terms currently stored   */
    static size_t cache_cap = 0;    /* capacity of the cached array       */

    /* First call: allocate space for at least the first 12 terms. */
    if (cache == NULL) {
        cache_cap = (size_t)(n > 12 ? n : 12);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) /* memory failure – return 0 as best effort */
            return (seq_t)0;

        for (size_t i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
        cache_len = 12;
    }

    /* Grow cache if required. */
    if ((size_t)n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (tmp == NULL)  /* reallocation failed – keep old buffer */
            return (seq_t)0;
        cache = tmp;
        cache_cap = (size_t)n;
    }

    /* Compute missing terms up to n. */
    while (cache_len < n) {
        idx_t term_no = cache_len + 1; /* 1‑based term number */

        if (term_no <= 12) {
            /* the initial values are already stored – nothing to do */
        } else {
            /* Recurrence: a(n) = 976*a(n-4) - 46848*a(n-8) + 110592*a(n-12) */
            seq_t part4  = cache[term_no - 5];   /* a(n‑4)  */
            seq_t part8  = cache[term_no - 9];   /* a(n‑8)  */
            seq_t part12 = cache[term_no - 13];  /* a(n‑12) */

            seq_t val = part4  * (seq_t)976
                      - part8  * (seq_t)46848
                      + part12 * (seq_t)110592;

            cache[cache_len] = val;
        }

        ++cache_len;
    }

    return cache[n - 1];
}
