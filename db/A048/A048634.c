/*
 * A048634  a(n) = a(n-1)*a(n-3) + a(n-2), with a(0)=a(1)=0 and a(2)=1.
 * https://oeis.org/A048634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*a(n-3) + a(n-2), a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 12, u16 n <= 13, u32 n <= 15, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static idx_t cache_cap   = 0;
    static idx_t last_index  = 2;   /* last computed index */

    /* initialise cache if necessary */
    if (cache == NULL) {
        idx_t init_cap = (n + 1) * 2;
        if (init_cap < 3) init_cap = 3;            /* guarantee space for a(0..2) */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache_cap = init_cap;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
    }
    else if (n >= cache_cap) {                    /* grow cache if needed */
        idx_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache     = tmp;
        cache_cap = new_cap;
    }

    if (n <= 2) return cache[n];

    for (idx_t i = last_index + 1; i <= n; ++i) {
        cache[i] = cache[i-1] * cache[i-3] + cache[i-2];
    }
    last_index = n;
    return cache[n];
}
