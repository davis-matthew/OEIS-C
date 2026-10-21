/*
 * A222342  Number of (n+5) X 1 arrays of occupancy after each element moves
 *          up to +-5 places including 0.
 * https://oeis.org/A222342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 45*a(n-2) + 84*a(n-3) - 70*a(n-4) + 21*a(n-5)
 *            - a(n-6) * Initial values: a(1)=462, a(2)=1714, a(3)=6405,
 *            a(4)=24038, a(5)=90440, a(6)=340746 * This implementation
 *            memoises the sequence in a static array. * It guarantees correct
 *            values for any seq_t (unsigned fixed‑width or arbi...
 * Range:     u8 none, u16 n <= 4, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, return 0 for safety */

    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;      /* capacity of cache array (indices 0..cap-1) */
    static idx_t max_calculated = 0; /* maximum index computed so far */

    /* initialise cache if necessary */
    if (cache == NULL)
    {
        /* at least allocate up to index 6 to hold the initial values */
        cache_cap = (n >= 6 ? n + 1 : 7);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* error: out of memory */
        cache[0] = 0;         /* unused */
        cache[1] = (seq_t)462;
        cache[2] = (seq_t)1714;
        cache[3] = (seq_t)6405;
        cache[4] = (seq_t)24038;
        cache[5] = (seq_t)90440;
        cache[6] = (seq_t)340746;
        max_calculated = 6;
    }

    /* grow cache if needed */
    if ((idx_t)n + 1 > cache_cap)
    {
        cache_cap = (idx_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* error: out of memory */
    }

    /* compute values up to requested n if not already done */
    for (idx_t i = max_calculated + 1; i <= n; ++i)
    {
        /* recurrence for i >= 7 */
        cache[i] =
            (seq_t)11 * cache[i - 1]
            - (seq_t)45 * cache[i - 2]
            + (seq_t)84 * cache[i - 3]
            - (seq_t)70 * cache[i - 4]
            + (seq_t)21 * cache[i - 5]
            -          cache[i - 6];
    }
    max_calculated = (n > max_calculated) ? n : max_calculated;

    return cache[n];
}
