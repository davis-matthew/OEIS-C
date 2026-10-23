/*
 * A115451  Expansion of 1/((1+x)*(1-2*x)*(1+x^2)).
 * https://oeis.org/A115451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + 2*a(n-4) Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values; grows as needed */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of terms stored (max index + 1) */

    /* grow the cache to hold up to index n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – in a contest setting we might abort */
            abort();
        }
        cache = new_cache;

        /* Initialise new entries using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0)          cache[i] = (seq_t)1;
            else if (i == 1)     cache[i] = (seq_t)1;
            else if (i == 2)     cache[i] = (seq_t)2;
            else if (i == 3)     cache[i] = (seq_t)4;
            else
                cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                         + ((seq_t)2 * cache[i-4]);
        }
        cache_size = new_size;
    }

    return cache[n];
}
