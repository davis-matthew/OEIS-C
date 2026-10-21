/*
 * A214103  Number of 0..2 colorings of a 4X(n+1) array circular in the n+1
 *          direction with new values 0..2 introduced in row major order.
 * https://oeis.org/A214103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            8*a(n-1)+26*a(n-2)-304*a(n-3)+304*a(n-4)+2016*a(n-5)-3991*a(n-6)-2616*a(n-7)+9702*a(n-8)-2064*a(n-9)-6120*a(n-10)+2880*a(n-11)
 *            Initial values: a(1)=27, a(2)=8, a(3)=771, a(4)=1080,
 *            a(5)=29540, a(6)=79968, a(7)=1241355, a(8)=4807928,
 *            a(9)=54733587, a(10)=263068168, a(11)=2491174972
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_len = 0; /* number of terms computed, cache[0] = a(1) */

    const seq_t init[11] = {27,8,771,1080,29540,79968,1241355,4807928,54733587,263068168,2491174972};

    if (n <= 11)
        return init[n-1];

    if (cache == NULL) {
        cache_cap = 12;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failure */
        for (idx_t i = 0; i < 11; ++i) cache[i] = init[i];
        cached_len = 11;                   /* a(1)..a(11) stored */
    }

    if ((idx_t)cache_cap < n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cached_len + 1; i <= n; ++i) {
        idx_t idx = i - 1; /* index in cache for a(i) */

        seq_t pos_sum = 8*cache[idx-1]      + 26*cache[idx-2]
                       + 304*cache[idx-4]   + 2016*cache[idx-5]
                       + 9702*cache[idx-8]  + 2880*cache[idx-11];
        seq_t neg_sum = 304*cache[idx-3]    + 3991*cache[idx-6]
                       + 2616*cache[idx-7]  + 2064*cache[idx-9]
                       + 6120*cache[idx-10];
        cache[idx] = pos_sum - neg_sum;
    }

    cached_len = n;
    return cache[n-1];
}
