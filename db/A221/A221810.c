/*
 * A221810  Number of 2Xn arrays of occupancy after each element moves to some
 *          horizontal, vertical or antidiagonal neighbor, without move-in
 *          move-out straight through or left turns.
 * https://oeis.org/A221810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +18*a(n-2) +22*a(n-3) -14*a(n-4) -39*a(n-5)
 *            -44*a(n-6) -6*a(n-7) +7*a(n-8) +3*a(n-9) +4*a(n-10) +a(n-11)
 *            (empirical 11‑term recurrence)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL; /* array 1 .. cache_cap */
    static size_t cache_cap  = 0;    /* allocated capacity (excluding 0) */
    static size_t cache_cur  = 0;    /* highest index that has been computed */

    /* first 11 terms given in OEIS */
    static const seq_t init[11] = {
        1, 10, 34, 214, 1016, 5429, 27535, 142858, 733811, 3785852, 19494390
    };

    /* initialise cache if necessary */
    if (!cache) {
        cache_cap = 22;                          /* room for future growth */
        cache    = seq_alloc_new_array(cache_cap + 1); /* indices 0..cache_cap */
        for (size_t i = 1; i <= 11; ++i) cache[i] = init[i-1];
        cache_cur = 11;                          /* we have computed up to 11 */
    }

    /* grow cache if it cannot hold requested index */
    if ((size_t)n > cache_cap) {
        size_t new_cap = n + 10;                           /* some slack */
        seq_t *tmp = seq_alloc_new_array(new_cap + 1);
        for (size_t i = 1; i <= cache_cur; ++i) tmp[i] = cache[i];
        seq_free_array(cache);
        cache    = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    if (n > (idx_t)cache_cur) {
        for (idx_t i = (idx_t)(cache_cur + 1); i <= n; ++i) {
            /* positive contributions */
            seq_t pos = cache[i-1];
            pos += (seq_t)18 * cache[i-2];
            pos += (seq_t)22 * cache[i-3];
            pos += (seq_t)7  * cache[i-8];
            pos += (seq_t)3  * cache[i-9];
            pos += (seq_t)4  * cache[i-10];
            pos += cache[i-11];

            /* negative contributions */
            seq_t neg = (seq_t)14 * cache[i-4];
            neg += (seq_t)39 * cache[i-5];
            neg += (seq_t)44 * cache[i-6];
            neg += (seq_t)6  * cache[i-7];

            cache[i] = pos - neg;
        }
        cache_cur = n;
    }

    return cache[(size_t)n];
}
