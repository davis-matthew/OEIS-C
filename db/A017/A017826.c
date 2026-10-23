/*
 * A017826  Expansion of
 *          1/(1-x^3-x^4-x^5-x^6-x^7-x^8-x^9-x^10-x^11-x^12-x^13).
 * https://oeis.org/A017826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 62, u64 n <= 121,
 *            u128 n <= 238, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-3)+a(n-4)+…+a(n-13) for n > 12
   Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=1,
   a(6)=2, a(7)=3, a(8)=4, a(9)=6, a(10)=9, a(11)=13, a(12)=19, a(13)=28 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Basic initial values up to a(13) */
    static const seq_t init[14] = {
        1, 0, 0, 1, 1, 1, 2,
        3, 4, 6, 9, 13, 19, 28
    };

    /* For n ≤ 13 just return the known value */
    if (n <= 13)
        return init[(size_t)n];

    /* Memoization cache */
    static idx_t cache_max = 0;      /* largest n for which cache[n] is set */
    static seq_t *cache = NULL;      /* array of computed a(i) */

    /* Ensure the cache is allocated large enough for indices 0…n */
    size_t needed = (size_t)n + 1;   /* number of elements required */

    if (cache == NULL) {
        /* First time: allocate space for at least n+1 elements, or 14 */
        size_t alloc = needed > 14 ? needed : 14;
        cache = (seq_t *)malloc(alloc * sizeof(seq_t));
        if (cache == NULL)
            return 0;               /* allocation failed – give up */

        /* Initialise the first 14 entries */
        for (idx_t i = 0; i <= 13; ++i)
            cache[i] = init[(size_t)i];
        cache_max = 13;
    }
    else if (needed > (size_t)(cache_max + 1)) {
        /* Grow the cache to accommodate larger n */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL)
            return 0;               /* allocation failed – give up */
        cache = tmp;
    }

    /* If we already have a(n), return it immediately */
    if (n <= cache_max)
        return cache[(size_t)n];

    /* Compute missing values from cache_max+1 up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* a(i) = a(i-3) + a(i-4) + … + a(i-13) */
        for (int k = 3; k <= 13; ++k)
            sum += cache[(size_t)(i - k)];
        cache[(size_t)i] = sum;
    }
    cache_max = n;
    return cache[(size_t)n];
}
