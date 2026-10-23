/*
 * A116798  Number of permutations of length n which avoid the patterns 1234,
 *          1342, 1432.
 * https://oeis.org/A116798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 15*a(n-2) + 11*a(n-3) - 3*a(n-4) *
 *            (recurrence, a(1)=1, a(2)=2, a(3)=6, a(4)=21)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t last_computed = 0;
    static size_t cache_cap = 0;

    /* undefined offset */
    if (n < 1) return 0;

    /* initialisation: first 4 terms */
    if (cache == NULL) {
        cache_cap = 5;                      /* indices 0..4 */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 6;
        cache[4] = 21;
        last_computed = 4;
    }

    /* grow the cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values by recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)7 * cache[i-1]
                 - (seq_t)15 * cache[i-2]
                 + (seq_t)11 * cache[i-3]
                 - (seq_t)3  * cache[i-4];
    }
    if (n > last_computed) last_computed = n;

    return cache[n];
}
