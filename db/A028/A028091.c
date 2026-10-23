/*
 * A028091  Expansion of 1/((1-3x)(1-7x)(1-8x)(1-11x)).
 * https://oeis.org/A028091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*a(n-1) - 299*a(n-2) + 1279*a(n-3) - 1848*a(n-4)
 *            Initial values: a(0)=1, a(1)=29, a(2)=542, a(3)=8326
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static size_t len     = 0;

    /* Ensure the array can hold up to n */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;

        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute any terms that are missing */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)29;
        } else if (i == 2) {
            val = (seq_t)542;
        } else if (i == 3) {
            val = (seq_t)8326;
        } else {
            val =   (seq_t)29   * cache[i-1]
                 - (seq_t)299  * cache[i-2]
                 + (seq_t)1279 * cache[i-3]
                 - (seq_t)1848 * cache[i-4];
        }
        cache[i] = val;
    }

    len = (size_t)n + 1;
    return cache[(size_t)n];
}
