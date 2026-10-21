/*
 * A060471  Smallest positive a(n) such that number of solutions to
 *          a(n)=a(j)+a(k) j<k<n is three or less.
 * https://oeis.org/A060471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) for n > 17 with initial terms
 *            a(1)=1, a(2)=2, ..., a(17)=30. The sequence is memoized;
 *            recurrence is applied iteratively up to any requested n
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Initial 17 terms.  Index 0 is unused. */
    static const seq_t init[18] = {
        0ULL,
        1ULL, 2ULL, 3ULL, 4ULL, 5ULL, 6ULL, 7ULL, 8ULL,
        10ULL, 12ULL, 14ULL, 16ULL, 19ULL, 21ULL, 23ULL, 25ULL, 30ULL
    };

    /* Cache of computed terms.  Indexed 1..cache_size. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* highest index currently stored */

    /* If cache not yet allocated or large enough, set up the first 17 terms. */
    if (cache == NULL) {
        cache_size = 17;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) abort();   /* unlikely, but safe */
        for (idx_t i = 1; i <= cache_size; ++i)
            cache[i] = init[i];
    }

    /* If requested term already in cache, return it. */
    if (n <= cache_size)
        return cache[n];

    /* Grow cache to hold up to n. */
    if (n > cache_size) {
        idx_t new_size = n;
        cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!cache) abort();

        /* Compute missing terms using the recurrence. */
        for (idx_t i = cache_size + 1; i <= new_size; ++i) {
            /* a(i) = a(i-1) + a(i-4) - a(i-5) */
            cache[i] = cache[i - 1] + cache[i - 4] - cache[i - 5];
        }
        cache_size = new_size;
    }

    return cache[n];
}
