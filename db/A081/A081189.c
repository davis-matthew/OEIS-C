/*
 * A081189  7th binomial transform of (1,0,1,0,1,...), A059841.
 * https://oeis.org/A081189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 48*a(n-2), a(0)=1, a(1)=7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* dynamically resized memoisation table */
static idx_t cache_size = 0;    /* number of elements currently stored in cache */

/* Compute the 7th binomial transform of (1,0,1,0,1,…)  (A081189) */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold n+1 elements */
    if (n >= cache_size) {
        /* new size: at least 2, otherwise n+1 */
        idx_t new_size = n + 1;
        if (new_size < 2) new_size = 2;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        /* initialise new slots */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                cache[i] = 1;                     /* a(0) */
            } else if (i == 1) {
                cache[i] = 7;                     /* a(1) */
            } else {
                /* recurrence: a(i) = 14*a(i-1) - 48*a(i-2) */
                cache[i] = cache[i-1] * (14) - cache[i-2] * (48);
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
