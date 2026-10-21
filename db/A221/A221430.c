/*
 * A221430  Number of nX2 arrays of occupancy after each element moves to some
 *          king-move neighbor, without consecutive moves in the same
 *          direction.
 * https://oeis.org/A221430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the 12‑term linear recurrence * * a(n) =
 *            13*a(n-1) - 13*a(n-2) - 151*a(n-3) + 286*a(n-4) * - 287*a(n-5) +
 *            833*a(n-6) - 966*a(n-7) + 488*a(n-8) * + 39*a(n-9) - 63*a(n-10)
 *            + 26*a(n-11) - 2*a(n-12) * * for n > 12, with the initial values
 *            * * a(1) = 1 * a(2) = 31 * a(3) = 286 * a(4) =...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[1] holds a(1), etc.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cached = 0;               /* highest index currently computed */
    static const seq_t init[12] = {
        (seq_t)1ULL,
        (seq_t)31ULL,
        (seq_t)286ULL,
        (seq_t)3295ULL,
        (seq_t)34477ULL,
        (seq_t)371116ULL,
        (seq_t)3952193ULL,
        (seq_t)42233189ULL,
        (seq_t)450737852ULL,
        (seq_t)4812507367ULL,
        (seq_t)51375286141ULL,
        (seq_t)548476573106ULL
    };

    /* Handle the trivial case n == 0 (outside the sequence). */
    if (n == 0)
        return (seq_t)0;

    /* Initialise the cache with the first 12 known values if not yet done. */
    if (cached == 0) {
        /* Allocate storage for indices 1..12 (index 0 unused). */
        cache = (seq_t *)malloc(13 * sizeof(seq_t));
        for (idx_t i = 1; i <= 12; ++i)
            cache[i] = init[i - 1];
        cached = 12;
    }

    /* If the requested index is already computed, return it immediately. */
    if (n <= cached)
        return cache[n];

    /* Grow the cache to hold terms up to n. */
    idx_t new_size = n + 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    /* Compute terms sequentially from (cached + 1) to n.  The recurrence
     * requires the previous 12 terms, which are available once cached >= 12. */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t v = 13ULL * cache[i - 1];
        v -= 13ULL * cache[i - 2];
        v -= 151ULL * cache[i - 3];
        v += 286ULL * cache[i - 4];
        v -= 287ULL * cache[i - 5];
        v += 833ULL * cache[i - 6];
        v -= 966ULL * cache[i - 7];
        v += 488ULL * cache[i - 8];
        v += 39ULL * cache[i - 9];
        v -= 63ULL * cache[i - 10];
        v += 26ULL * cache[i - 11];
        v -= 2ULL * cache[i - 12];
        cache[i] = v;
    }

    cached = n;
    return cache[n];
}
