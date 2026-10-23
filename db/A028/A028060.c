/*
 * A028060  Expansion of 1/((1-3x)(1-5x)(1-7x)(1-9x)).
 * https://oeis.org/A028060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9^(n+3)-3*7^(n+3)+3*5^(n+3)-3^(n+3))/48 Recurrence: a(n)
 *            = 24*a(n-1)-206*a(n-2)+744*a(n-3)-945*a(n-4) Initial values:
 *            a(0)=1, a(1)=24, a(2)=370, a(3)=4680
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* grow cache if required */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise initial values only once */
    if (cache_size == 0) {
        if (cache_cap < 4) {
            seq_t *tmp = (seq_t *)realloc(cache, 4 * sizeof(seq_t));
            cache = tmp;
            cache_cap = 4;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
        cache[2] = (seq_t)370;
        cache[3] = (seq_t)4680;
        cache_size = 4;
    }

    /* return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* compute values up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        __int128 tmp = 24 * (__int128)cache[i - 1]
                     - 206 * (__int128)cache[i - 2]
                     + 744 * (__int128)cache[i - 3]
                     - 945 * (__int128)cache[i - 4];
        cache[i] = (seq_t)tmp;
    }
    cache_size = n + 1;
    return cache[n];
}
