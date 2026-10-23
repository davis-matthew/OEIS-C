/*
 * A021509  Decimal expansion of 1/505.
 * https://oeis.org/A021509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy, changed
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) for n > 3 with a(0)=0, a(1)=0,
 *            a(2)=1, a(3)=9
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* array holding computed terms */
    static idx_t  cache_len = 0;       /* number of terms stored so far */
    static idx_t  cache_cap = 0;       /* current capacity of the array */

    /* First call: allocate an initial buffer */
    if (!cache) {
        cache_cap = 16; /* arbitrary small initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure: return 0 */
        for (idx_t i = 0; i < cache_cap; ++i) cache[i] = 0;
    }

    /* Grow the buffer if the requested index is beyond current capacity */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;  /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        /* zero the newly allocated part (not strictly needed) */
        for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to and including n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0 || i == 1) {
                cache[i] = 0;
            } else if (i == 2) {
                cache[i] = 1;
            } else if (i == 3) {
                cache[i] = 9;
            } else {
                /* a(n) = a(n-1) - a(n-2) + a(n-3)
                 * but compute as (a(n-1)+a(n-3)) - a(n-2) to avoid unsigned underflow */
                cache[i] = (cache[i-1] + cache[i-3]) - cache[i-2];
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
