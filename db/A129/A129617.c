/*
 * A129617  a(1) = 0; a(2) = a(3) = 1; for n > 3: a(n) = 16*a(n-2)+14*a(n-3).
 * https://oeis.org/A129617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0; a(2) = a(3) = 1; for n > 3: a(n) = 16*a(n-2) +
 *            14*a(n-3);
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;   /* undefined for n < 1 */
    }

    /* static cache: indices 1..max stored in cache[1..max] */
    static seq_t *cache  = NULL;
    static size_t  cap   = 0;   /* allocated size (in elements) */
    static size_t  max   = 0;   /* largest n computed so far */

    /* Ensure cache has room for index n */
    if (n + 1 > cap) {            /* +1 to allow direct indexing at n */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; abort rather than silently wrong value */
            if (cache) free(cache);
            abort();
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute values up to n if needed */
    for (idx_t i = (idx_t)max + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)0;
        } else if (i == 2 || i == 3) {
            cache[i] = (seq_t)1;
        } else {
            /* a(i) = 16*a(i-2) + 14*a(i-3) */
            cache[i] = (seq_t)16 * cache[i - 2] + (seq_t)14 * cache[i - 3];
        }
    }
    if (n > max) {
        max = (size_t)n;
    }

    return cache[n];
}
