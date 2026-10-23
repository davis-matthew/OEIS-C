/*
 * A176380  x-values in the solution to x^2-71*y^2=1.
 * https://oeis.org/A176380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 3480, a(n) = 6960*a(n-1)-a(n-2) for n >= 3 OEIS
 *            sequence A176380
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* index 0 is unused */
    if (n < 1)
        return 0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n > cache_size) {
        /* enlarge cache to include index n */
        size_t new_len = (size_t)n + 1;          /* indices 0 .. n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return 0;                          /* allocation failure */
        cache = tmp;

        /* compute all missing terms up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
                continue;
            }
            if (i == 2) {
                cache[2] = (seq_t)3480;
                continue;
            }
            cache[i] = ((seq_t)6960) * cache[i - 1] - cache[i - 2];
        }
        cache_size = n;
    }

    return cache[n];
}
