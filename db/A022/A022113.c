/*
 * A022113  Fibonacci sequence beginning 2, 7.
 * https://oeis.org/A022113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2); a(0) = 2; a(1) = 7
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t size  = 0;      /* number of terms stored */

    /* make sure we can store term n */
    if (cap < (size_t)n + 1) {
        size_t new_cap = cap ? cap : 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* initialise base values if needed */
    if (size == 0) {              /* haven't stored anything yet */
        cache[0] = (seq_t)2;
        size = 1;
    }
    if (size == 1) {
        cache[1] = (seq_t)7;
        size = 2;
    }

    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* compute missing terms up to n */
    idx_t i;
    for (i = (idx_t)size; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2];
    }
    size = (size_t)n + 1;
    return cache[n];
}
