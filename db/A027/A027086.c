/*
 * A027086  a(n) = A027082(n, n+4).
 * https://oeis.org/A027086
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)-5*a(n-2)+2*a(n-3)-a(n-4)+2*a(n-5)-a(n-6) for n >
 *            9, with initial values: a(4) = 11, a(5) = 41, a(6) = 108, a(7) =
 *            246, a(8) = 517, a(9) = 1035. Offset is 4 (a(4) is the first
 *            term)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 33, u64 n <= 70, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;     /* storage of computed values   */
static size_t cache_len  = 0;        /* number of values stored      */
static size_t cache_cap  = 0;        /* allocated array size         */

static const seq_t init[6] = { (seq_t)11, (seq_t)41, (seq_t)108,
                               (seq_t)246, (seq_t)517, (seq_t)1035 };

/* sequence a(n) for n >= 4 */
seq_t a(idx_t n)
{
    if (n < 4)                /* below the offset – behaviour undefined */
        return 0;

    const size_t idx = (size_t)(n - 4);   /* zero‑based index in cache */

    /* if the requested term is one of the initial values, return it directly */
    if (idx < 6)
        return init[idx];

    /* Allocate the cache once */
    if (cache == NULL) {
        /* at least up to idx, but start with a small capacity */
        size_t need = idx + 1;
        size_t initcap = (need > 16 ? need : 16);
        cache = (seq_t *)malloc(initcap * sizeof(seq_t));
        if (!cache)
            return 0;               /* out of memory – return 0 */

        /* initialise the first six values */
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init[i];

        cache_len = 6;
        cache_cap = initcap;
    }

    /* Grow the allocated array if necessary */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap;
        while (idx >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;           /* allocation failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to idx */
    while (cache_len <= idx) {
        /* recurrence: a(n) = 4*a(n-1)-5*a(n-2)+2*a(n-3)-a(n-4)+2*a(n-5)-a(n-6) */
        seq_t t = (seq_t)0;
        t = (seq_t)4 * cache[cache_len - 1];
        t -= (seq_t)5 * cache[cache_len - 2];
        t += (seq_t)2 * cache[cache_len - 3];
        t -= (seq_t)1 * cache[cache_len - 4];
        t += (seq_t)2 * cache[cache_len - 5];
        t -= (seq_t)1 * cache[cache_len - 6];

        cache[cache_len] = t;
        ++cache_len;
    }

    return cache[idx];
}
