/*
 * A232771  Values of x satisfying x^2 = floor(y^2/3 + y).
 * https://oeis.org/A232771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-3) - a(n-6) for n >= 7, with seeds a(1)=0, a(2)=1,
 *            a(3)=3, a(4)=6, a(5)=17, a(6)=43
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function memoizes computed terms in a static cache. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached = 0;
    static const seq_t seeds[6] = {0, 1, 3, 6, 17, 43};

    if (n <= 6)
        return seeds[n - 1];

    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        if (!cache) exit(1);
        for (idx_t i = 1; i <= n; ++i) {
            if (i <= 6)
                cache[i - 1] = seeds[i - 1];
            else
                cache[i - 1] = 14 * cache[i - 4] - cache[i - 7];
        }
        cached = n;
        return cache[n - 1];
    }

    if (n <= cached)
        return cache[n - 1];

    cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!cache) exit(1);
    for (idx_t i = cached + 1; i <= n; ++i)
        cache[i - 1] = 14 * cache[i - 4] - cache[i - 7];
    cached = n;
    return cache[n - 1];
}
