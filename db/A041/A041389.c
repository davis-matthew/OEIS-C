/*
 * A041389  Denominators of continued fraction convergents to sqrt(209).
 * https://oeis.org/A041389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041389: Denominators of continued fraction convergents to sqrt(209).
 * Recurrence: a(n) = 93102*a(n-8) - a(n-16)  for n > 15.
 * Initial terms (n = 0 … 15):
 *   1, 2, 11, 35, 81, 278, 1471, 3220, 91631, 186482,
 *   1024041, 3258605, 7541251, 25882358, 136953041, 299788440.
 */
#include <stdlib.h>

/* seeds for n = 0 … 15 */
static const seq_t init_vals[16] = {
    (seq_t)1,   (seq_t)2,   (seq_t)11,   (seq_t)35,
    (seq_t)81,  (seq_t)278,  (seq_t)1471, (seq_t)3220,
    (seq_t)91631, (seq_t)186482, (seq_t)1024041, (seq_t)3258605,
    (seq_t)7541251, (seq_t)25882358, (seq_t)136953041, (seq_t)299788440
};

seq_t a(idx_t n)
{
    if (n < 16)
        return init_vals[n];

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements stored */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* lazy initialisation of the cache */
    if (!cache) {
        cache_cap = 64;                               /* start with a modest size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)                  /* allocation failure – return zero */
            return (seq_t)0;
        for (idx_t i = 0; i < 16; ++i)
            cache[i] = init_vals[i];
        cache_len = 16;
    }

    /* value already known? */
    if (n < cache_len)
        return cache[n];

    /* ensure the array is large enough */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;                       /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                                 /* allocation failure – return zero */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i)
        cache[i] = (seq_t)93102 * cache[i - 8] - cache[i - 16];

    cache_len = n + 1;   /* update number of stored elements */
    return cache[n];
}
