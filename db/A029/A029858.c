/*
 * A029858  a(n) = (3^n - 3)/2.
 * https://oeis.org/A029858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n - 3)/2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 0; a(n) = 3*a(n-1) + 3 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; n>=1 expected */

    /* dynamic cache: cache[i] holds a(i+1) (since a(1) is at index 0) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of entries stored */
    static size_t cache_cap = 0;          /* allocated capacity */

    /* allocate / grow cache if needed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;
        /* initialise new entries */
        if (cache_len == 0) {
            /* first element: a(1) = 0 */
            cache[0] = (seq_t)0;
            cache_len = 1;
        }
        for (size_t i = cache_len; i < new_cap; ++i) {
            /* recurring formula: a(i+1) = 3*a(i) + 3 */
            cache[i] = ((seq_t)3) * cache[i - 1] + (seq_t)3;
        }
        cache_len = new_cap;
        cache_cap = new_cap;
    }

    return cache[n - 1];
}
