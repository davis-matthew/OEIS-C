/*
 * A137249  Expansion of g.f. z*(2-2*z+z^2+z^3)/((1+z)*(1-3*z+2*z^2-z^3)).
 * https://oeis.org/A137249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+4) = 2*a(n+3) + a(n+2) - a(n+1) + a(n);
   Initial values: a(1)=2, a(2)=2, a(3)=7, a(4)=15 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Sequence is defined only for n >= 1 */
    if (n < 1)
        return 0;

    /* Static cache shared between calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements currently allocated */
    static idx_t last = 0;         /* highest index that has been computed */

    /* Ensure we have room for at least max(n,4)+1 elements (indices 0..max) */
    idx_t needed = (n > (idx_t)4 ? n : (idx_t)4) + 1;
    if (cache_size < needed)
    {
        size_t new_size = (size_t)needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0;              /* allocation failed */
        cache = tmp;
        cache_size = needed;
    }

    /* Initialise the first four terms once */
    if (last == 0)
    {
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)15;
        last = 4;
    }

    /* Return immediately if the value is already computed */
    if (n <= last)
        return cache[n];

    /* Compute all missing values up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        seq_t val = 2 * cache[i - 1];
        val += cache[i - 2];
        val -= cache[i - 3];
        val += cache[i - 4];
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
