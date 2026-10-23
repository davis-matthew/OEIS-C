/*
 * A107477  a(n)= 7*a(n-1) +5*a(n-2) -35*a(n-3) +7*a(n-4) +5*a(n-5) -a(n-6),
 *          n>10.
 * https://oeis.org/A107477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 5*a(n-2) - 35*a(n-3) + 7*a(n-4) + 5*a(n-5) -
 *            a(n-6) for n > 10 Initial values: a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=5, a(5)=8, a(6)=118, a(7)=1011, a(8)=7339,
 *            a(9)=52332, a(10)=368505
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* First 11 terms of the sequence, offset 0 */
    static const seq_t init[] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5,
        (seq_t)8, (seq_t)118, (seq_t)1011, (seq_t)7339,
        (seq_t)52332, (seq_t)368505
    };
    static const idx_t init_len = sizeof(init) / sizeof(init[0]);

    /* Direct return for indices below the initial prefix */
    if (n < init_len)
        return init[n];

    /* Memoisation buffer */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;   /* number of elements currently stored */

    /* Initialise the cache on first use */
    if (cache == NULL) {
        cache_size = init_len;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure – unlikely */
        for (idx_t i = 0; i < init_len; ++i)
            cache[i] = init[i];
    }

    /* Extend the cache up to the requested index n */
    while (cache_size <= n) {
        idx_t i = cache_size;                /* index of the new term */

        /* Compute the recurrence */
        seq_t term = cache[i-1] * (seq_t)7
                   + cache[i-2] * (seq_t)5
                   - cache[i-3] * (seq_t)35
                   + cache[i-4] * (seq_t)7
                   + cache[i-5] * (seq_t)5
                   - cache[i-6];

        /* Grow the cache to accommodate the new term */
        seq_t *tmp = (seq_t *)realloc(cache,
                                    (cache_size + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – unlikely */
        cache = tmp;
        cache[cache_size] = term;
        ++cache_size;
    }

    return cache[n];
}
