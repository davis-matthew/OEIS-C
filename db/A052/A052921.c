/*
 * A052921  Expansion of (1 - x)/(1 - 3*x + 2*x^2 - x^3).
 * https://oeis.org/A052921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3), a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached terms – allows arbitrary call order. */
    static seq_t *cache = NULL;  /* array of computed terms       */
    static idx_t  cap   = 0;     /* current capacity (elements)  */
    static idx_t  len   = 0;     /* number of terms stored       */

    /* Grow the cache if needed.  Always start with at least 4 entries. */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? cap * 2 : 4;
        while (new_cap <= n)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Initialise the first three terms once. */
    if (len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        len = 3;
    }

    /* Compute terms up to index n. */
    while (len <= n) {
        seq_t val = (seq_t)3 * cache[len - 1];
        val -= (seq_t)2 * cache[len - 2];
        val += cache[len - 3];
        cache[len++] = val;
    }

    return cache[n];
}
