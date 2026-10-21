/*
 * A081896  Third binomial transform of binomial(n+3, 3).
 * https://oeis.org/A081896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n*(n^3 + 33*n^2 + 254*n + 384)/384 Recurrence (rational
 *            G.f.): a(n) = 16*a(n-1) - 96*a(n-2) + 256*a(n-3) - 256*a(n-4),
 *            with a(0)=1, a(1)=7, a(2)=43, a(3)=245
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    /* Ensure cache has at least 4 elements initialized */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)43;
        cache[3] = (seq_t)245;
        max_cached = 3;
    }

    /* Expand cache up to n if necessary */
    if (n > max_cached) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            abort(); /* allocation failure */
        cache = tmp;
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            /* recurrence: a(i) = 16*a(i-1) - 96*a(i-2) + 256*a(i-3) - 256*a(i-4) */
            cache[i] = (seq_t)16 * cache[i - 1] 
                     - (seq_t)96 * cache[i - 2] 
                     + (seq_t)256 * cache[i - 3] 
                     - (seq_t)256 * cache[i - 4];
        }
        max_cached = n;
    }

    return cache[n];
}
