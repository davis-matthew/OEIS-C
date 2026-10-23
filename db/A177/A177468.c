/*
 * A177468  Expansion of g.f.:
 *          (1+x+12*x^2-8*x^3)/(1-5*x-30*x^2+69*x^3+31*x^4-22*x^5).
 * https://oeis.org/A177468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1)+30*a(n-2)-69*a(n-3)-31*a(n-4)+22*a(n-5) with
 *            a(1)=1, a(2)=6, a(3)=72, a(4)=463, a(5)=4030
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache where cache[0] == a(1) */
    static seq_t *cache    = NULL;
    static size_t cache_len   = 0;
    static size_t cache_cap   = 0;

    /* first allocation: 8 slots, store the first five terms */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)6;   /* a(2) */
        cache[2] = (seq_t)72;  /* a(3) */
        cache[3] = (seq_t)463; /* a(4) */
        cache[4] = (seq_t)4030;/* a(5) */
        cache_len = 5;
    }

    /* enlarge cache if necessary */
    size_t need = (size_t)n;
    if (need > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < need) new_cap = need;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* term already computed? */
    if (need <= cache_len) {
        return cache[need - 1];
    }

    /* generate terms up to n using the linear recurrence */
    for (size_t i = cache_len; i < need; ++i) {
        cache[i] = (seq_t)5  * cache[i-1]
                 + (seq_t)30 * cache[i-2]
                 - (seq_t)69 * cache[i-3]
                 - (seq_t)31 * cache[i-4]
                 + (seq_t)22 * cache[i-5];
    }

    cache_len = need;
    return cache[need - 1];
}
