/*
 * A116566  Expansion of (-59*x + 1)/(-899*x^3 - 930*x^2 - 60*x + 1), modulo
 *          37.
 * https://oeis.org/A116566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, obsc, less
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = (60*a(n-1) + 930*a(n-2) + 899*a(n-3)) mod 37
   with a(0) = 1, a(1) = 1, a(2) = 28.  This is the power‑series
   expansion of (-59*x + 1)/(-899*x^3 - 930*x^2 - 60*x + 1) modulo 37. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t  *cache = NULL;
    static idx_t   cache_size = 0;
    static idx_t   last_computed = (idx_t)-1;   /* sentinel for uninitialized */

    /* Resize cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = cache_size ? cache_size << 1 : 4;
        while (new_size <= n + 1) new_size <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise first three terms on first call */
    if (last_computed == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)28;
        last_computed = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = ( (60 * cache[i-1] + 930 * cache[i-2] + 899 * cache[i-3]) )
                   % (seq_t)37;
    }
    if (n > last_computed) last_computed = n;

    return cache[n];
}
