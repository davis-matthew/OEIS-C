/*
 * A192806  a(n) = 7*a(n-1) - 5*a(n-2) + a(n-3), with initial values a(0) =
 *          a(1) = 1, a(2)=4.
 * https://oeis.org/A192806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 5*a(n-2) + a(n-3), a(0)=1, a(1)=1, a(2)=4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t last_computed = 2;   /* highest index already computed */

    /* we need at least three slots for the initial values */
    size_t needed = (size_t)n + 1;
    if (needed < 3) needed = 3;

    /* grow the cache if necessary and initialise the base cases */
    if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();            /* out‑of‑memory */
        cache = tmp;
        cache_size = needed;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
    } else if (cache_size < 3) {      /* first allocation never reached here, but keep safe */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
    }

    if (n < 3)                     /* already known */
        return cache[(size_t)n];

    if (last_computed < 2)
        last_computed = 2;

    /* compute any missing terms up to n */
    if ((idx_t)n > last_computed) {
        for (size_t i = (size_t)last_computed + 1; i <= (size_t)n; ++i) {
            seq_t v = (seq_t)7 * cache[i-1];
            v = v - (seq_t)5 * cache[i-2];
            v = v + cache[i-3];
            cache[i] = v;
        }
        last_computed = n;
    }

    return cache[(size_t)n];
}
