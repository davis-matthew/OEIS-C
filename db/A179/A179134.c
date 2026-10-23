/*
 * A179134  a(n) = (F(2*n-1) + F(2*n+2)) * (5/6 - cos(2*Pi*n/3)/3), where F(n)
 *          = Fibonacci(n).
 * https://oeis.org/A179134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 18*a(n-3) - a(n-6)
   Initial values: a(0)=1, a(1)=4, a(2)=10, a(3)=13, a(4)=68, a(5)=178
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static const seq_t init[6] = {1, 4, 10, 13, 68, 178};

    /* initialise cache on first call */
    if (cache_size == 0) {
        cache_size = 6;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
    }

    /* extend cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – return 0 as a fallback */
            return (seq_t)0;
        }
        cache = new_cache;

        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 6) {
                cache[i] = init[i];
            } else {
                cache[i] = ((seq_t)18) * cache[i-3] - cache[i-6];
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
