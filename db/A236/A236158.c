/*
 * A236158  Number of (n+1) X (2+1) 0..2 arrays colored with the sets of
 *          distinct values in every 2 X 2 subblock, with new values 0..2
 *          introduced row-major order.
 * https://oeis.org/A236158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 19*a(n-2) + 24*a(n-3) - a(n-4) - 22*a(n-5) -
 *            11*a(n-6) - 2*a(n-7) - 2*a(n-8) for n>8 Initial values: a(1) =
 *            69 a(2) = 417 a(3) = 2688 a(4) = 17406 a(5) = 112830 a(6) =
 *            731610 a(7) = 4743680 a(8) = 30758256
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap   = 0;  /* capacity of the array */
    static idx_t cache_len   = 0;  /* number of terms actually stored */

    /* Initialise the cache with the first 8 terms on the first call */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)69;
        cache[1] = (seq_t)417;
        cache[2] = (seq_t)2688;
        cache[3] = (seq_t)17406;
        cache[4] = (seq_t)112830;
        cache[5] = (seq_t)731610;
        cache[6] = (seq_t)4743680;
        cache[7] = (seq_t)30758256;
        cache_len = 8;                 /* we have stored 8 terms */
    }

    /* Return the value if it has already been computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the storage if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        if (new_cap == 0)
            new_cap = 8;
        while (new_cap < n)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t term =
            (seq_t)3  * cache[i - 2] +
            (seq_t)19 * cache[i - 3] +
            (seq_t)24 * cache[i - 4] -
            cache[i - 5] -
            (seq_t)22 * cache[i - 6] -
            (seq_t)11 * cache[i - 7] -
            (seq_t)2  * cache[i - 8] -
            (seq_t)2  * cache[i - 9];
        cache[i - 1] = term;
    }
    cache_len = n;          /* update number of stored terms */
    return cache[n - 1];
}
