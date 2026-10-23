/*
 * A186566  Number of (n+2)X9 0..2 arrays with every 3X3 subblock commuting
 *          with each horizontal and vertical neighbor 3X3 subblock.
 * https://oeis.org/A186566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence (valid for n > 15) a(n) = 2*a(n-1) - a(n-2)
 *            + 2*a(n-3) - 4*a(n-4) + 2*a(n-5) + 2*a(n-6) - 2*a(n-7) Initial
 *            values (n = 1 .. 15): a(1) = 29303 a(9) = 82447 a(2) = 4033
 *            a(10) = 122725 a(3) = 6511 a(11) = 185863 a(4) = 14845 a(12) =
 *            278125 a(5) = 27391 a(13) = 425791 a(6) = 3613...
 * Range:     u8 none, u16 n <= 8, u32 n <= 34, u64 n <= 86, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static bool initialized = false;

    /* Known values for n = 0 .. 15 (index 0 unused) */
    static const seq_t seeds[16] = {
        0ULL,          /* a(0) placeholder */
        29303ULL,      /* a(1) */
        4033ULL,       /* a(2) */
        6511ULL,       /* a(3) */
        14845ULL,      /* a(4) */
        27391ULL,      /* a(5) */
        36133ULL,      /* a(6) */
        47719ULL,      /* a(7) */
        60973ULL,      /* a(8) */
        82447ULL,      /* a(9) */
        122725ULL,     /* a(10) */
        185863ULL,     /* a(11) */
        278125ULL,     /* a(12) */
        425791ULL,     /* a(13) */
        645685ULL,     /* a(14) */
        993687ULL      /* a(15) */
    };
    const idx_t seed_count = 16;   /* indices 0 .. 15 inclusive */

    /* initialise cache once */
    if (!initialized) {
        cache_len = seed_count;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < seed_count; ++i)
            cache[i] = seeds[i];
        initialized = true;
    }

    /* return directly if requested value is already in cache */
    if (n < cache_len)
        return cache[n];

    /* grow cache to hold values up to n */
    idx_t old_len = cache_len;
    idx_t new_len = n + 1;         /* indices 0 .. n */
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (new_cache == NULL)
        exit(1);                   /* allocation failure */
    cache = new_cache;
    cache_len = new_len;

    /* compute missing values using the empirical recurrence */
    for (idx_t i = old_len; i <= n; ++i) {
        /* recurrence valid for i > 15; for i <= 15 cache already contains seeds */
        cache[i] = 2 * cache[i-1]
                 - cache[i-2]
                 + 2 * cache[i-3]
                 - 4 * cache[i-4]
                 + 2 * cache[i-5]
                 + 2 * cache[i-6]
                 - 2 * cache[i-7];
    }

    return cache[n];
}
