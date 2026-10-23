/*
 * A188100  Number of (n+3) X 7 binary arrays with every 4 X 4 subblock
 *          commuting with each horizontal and vertical neighbor 4 X 4
 *          subblock.
 * https://oeis.org/A188100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-4) - 2*a(n-5) + a(n-6) + a(n-8) -
 *            a(n-9) for n > 14 with initial values: a(1)=3594, a(2)=643,
 *            a(3)=1421, a(4)=3341, a(5)=5037, a(6)=5661, a(7)=5953,
 *            a(8)=6253, a(9)=6581, a(10)=7077, a(11)=8093, a(12)=10545,
 *            a(13)=15169, a(14)=21210
 * Range:     u8 none, u16 n <= 18, u32 n <= 63, u64 n <= 152, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0; /* number of allocated elements (indices 0..cache_size-1) */

    /* Initialise cache with the first 14 non‑zero terms. */
    if (cache == NULL) {
        cache_size = 15; /* indices 0..14 (index 0 unused) */
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (cache == NULL) abort();

        static const seq_t seed[15] = {
            0ULL,        /* dummy for index 0 */
            3594ULL, 643ULL, 1421ULL, 3341ULL, 5037ULL,
            5661ULL, 5953ULL, 6253ULL, 6581ULL, 7077ULL,
            8093ULL, 10545ULL, 15169ULL, 21210ULL
        };
        for (size_t i = 1; i < cache_size; ++i)
            cache[i] = seed[i];
    }

    /* Grow the cache if the requested index is beyond the current size. */
    if ((size_t)n >= cache_size) {
        size_t old_size = cache_size;
        size_t new_size = (size_t)n + 1;

        seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (new_cache == NULL) abort();

        memcpy(new_cache, cache, old_size * sizeof(seq_t));

        free(cache);
        cache = new_cache;
        cache_size = new_size;

        /* Compute the remaining terms using the recurrence. */
        for (size_t i = old_size; i < new_size; ++i) {
            if (i <= 14) continue; /* already seeded */

            seq_t val = 2 * cache[i - 1];
            val -= cache[i - 2];
            val += cache[i - 4];
            val -= 2 * cache[i - 5];
            val += cache[i - 6];
            val += cache[i - 8];
            val -= cache[i - 9];
            cache[i] = val;
        }
    }

    return cache[(size_t)n];
}
