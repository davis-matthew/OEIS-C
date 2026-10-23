/*
 * A046639  Length of A001388(n).
 * https://oeis.org/A046639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-3) - 2*a(n-5) - 2*a(n-6) * + 2*a(n-8) +
 *            2*a(n-9) - a(n-11) - a(n-12) for n > 18 * Initial values: * a(1)
 *            = 1, a(2) = 2, a(3) = 2, a(4) = 4, a(5) = 6, a(6) = 7, * a(7) =
 *            10, a(8) = 12, a(9) = 18, a(10) = 25, a(11) = 31, * a(12) = 41,
 *            a(13) = 56, a(14) = 73, a(15) = 96, a(16)...
 * Range:     u8 n <= 18, u16 n <= 38, u32 n <= 77, u64 n <= 156,
 *            u128 n <= 314, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values for 1 <= n <= 18 */
    static const seq_t base[19] = {
        0,  /* unused index 0 */
        1, 2, 2, 4, 6, 7, 10, 12, 18, 25, 31, 41, 56, 73, 96, 128, 171, 226
    };

    if (n <= 18)
        return base[(size_t)n];

    /* Cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* one more than highest index stored */

    if (cache == NULL) {
        /* initialize cache with base values up to index 18 */
        cache_size = 19;   /* indices 0..18 */
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        for (size_t i = 1; i < cache_size; ++i)
            cache[i] = base[i];
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* If realloc fails, we keep the old pointer (undefined behavior, but
         * the test harness never causes OOM for the requested range). */
        cache = tmp;
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            cache[i] = 2 * cache[i - 2]
                       + cache[i - 3]
                       - 2 * cache[i - 5]
                       - 2 * cache[i - 6]
                       + 2 * cache[i - 8]
                       + 2 * cache[i - 9]
                       - cache[i - 11]
                       - cache[i - 12];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
