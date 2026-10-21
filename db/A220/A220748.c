/*
 * A220748  Equals one maps: number of nX3 binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          horizontal and vertical neighbors in a random 0..3 nX3 array.
 * https://oeis.org/A220748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 3*a(n-2) - 24*a(n-3) - 3*a(n-4) + 24*a(n-5) +
 *            a(n-6) - 8*a(n-7), for n > 14 Initial terms (offset 1): a(1) =
 *            4, a(2) = 38, a(3) = 395, a(4) = 3778, a(5) = 32178, a(6) =
 *            261356, a(7) = 2096110, a(8) = 16776148, a(9) = 134216342, a(10)
 *            = 1073740504, a(11) = 8589932829, a(12) = 687...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* realloc, abort */

seq_t a(idx_t n) {
    /* Internal static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* highest index cached (inclusive) */
    static idx_t cache_cap = 0;    /* allocated capacity */

    if (n == 0) return 0; /* behaviour below offset is undefined */

    /* Ensure cache is large enough and filled up to index n */
    if (n > cache_size) {
        /* Grow capacity if necessary */
        if (n + 1 > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            while (new_cap < (size_t)(n + 1))
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* memory allocation failed */
            cache = tmp;
            cache_cap = (idx_t)new_cap;
        }

        /* Fill new entries */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* Base values for i = 1 .. 14 */
            if (i <= 14) {
                static const seq_t base[15] = {
                    0ULL,           /* index 0 unused */
                    4ULL, 38ULL, 395ULL, 3778ULL,
                    32178ULL, 261356ULL, 2096110ULL,
                    16776148ULL, 134216342ULL,
                    1073740504ULL, 8589932829ULL,
                    68719475152ULL, 549755811711ULL,
                    4398046509244ULL
                };
                cache[i] = base[i];
            } else {
                /* Recurrence for i > 14 */
                seq_t pos = 8 * cache[i-1] + 3 * cache[i-2]
                          + 24 * cache[i-5] + cache[i-6];
                seq_t neg = 24 * cache[i-3] + 3 * cache[i-4]
                          + 8 * cache[i-7];
                cache[i] = pos - neg;
            }
        }
        cache_size = n;
    }

    return cache[n];
}
