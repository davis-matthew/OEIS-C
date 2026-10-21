/*
 * A229388  Number of nX4 0..3 arrays of the sum of the corresponding element,
 *          the element to the east and the element to the south in a larger
 *          (n+1)X5 0..1 array.
 * https://oeis.org/A229388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 49*a(n-1) - 560*a(n-2) + 544*a(n-3) - 1568*a(n-4) +
 *            17920*a(n-5) - 16384*a(n-6) for n>9 initial values: a(1) = 164
 *            a(2) = 8592 a(3) = 319340 a(4) = 10624396 a(5) = 345788172 a(6)
 *            = 11156280332 a(7) = 358453456908 a(8) = 11493734735884 a(9) =
 *            368171037655052
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size (indices 0..cache_cap-1) */
    static idx_t cache_len = 0;   /* highest index stored */

    /* Constant initial values a(1)..a(9) */
    static const seq_t init[10] = {
        (seq_t)0,
        (seq_t)164,
        (seq_t)8592,
        (seq_t)319340,
        (seq_t)10624396,
        (seq_t)345788172,
        (seq_t)11156280332,
        (seq_t)358453456908,
        (seq_t)11493734735884,
        (seq_t)368171037655052
    };

    /* For n <= 9, just return the pre‑computed value */
    if (n <= 9)
        return init[n];

    /* Initialize cache if needed */
    if (cache_len < 9)
    {
        /* Allocate space for at least 10 indices (0..9) */
        cache_cap = 16;
        if (cache_cap < n + 1)
            cache_cap = n + 1;          /* ensure room for the requested n */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            return (seq_t)0;            /* allocation failed – unlikely */

        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = init[i];
        cache_len = 9;
    }

    /* Grow cache if larger n is requested */
    if (n >= cache_cap)
    {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp)
            return (seq_t)0;            /* reallocation failed – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values with the recurrence
       Cache[n] will be set for all i from cache_len+1 up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t val = (seq_t)49 * cache[i - 1];
        val += (seq_t)544 * cache[i - 3];
        val += (seq_t)17920 * cache[i - 5];
        val -= (seq_t)560 * cache[i - 2];
        val -= (seq_t)1568 * cache[i - 4];
        val -= (seq_t)16384 * cache[i - 6];
        cache[i] = val;
    }
    cache_len = n;
    return cache[n];
}
