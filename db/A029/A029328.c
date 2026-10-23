/*
 * A029328  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^6)*(1-x^9)).
 * https://oeis.org/A029328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4)+a(n-5)+a(n-6)-a(n-10)-a(n-11)-a(n-13)-a(n-14)
 *            +a(n-18)+a(n-19)+a(n-20)-a(n-24) Initial values: a(0)=1, a(1)=0,
 *            a(2)=0, a(3)=0, and a(4)=a(5)=a(6)=1
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;        /* pointer to allocated array */
    static size_t cache_cap = 0;       /* current allocated capacity */
    static idx_t  cache_used = 0;      /* number of entries computed (last index + 1) */

    /* ensure enough capacity */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* out of memory – fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing entries */
    for (idx_t i = cache_used; i <= n; ++i) {
        size_t idx = (size_t)i;
        if (i == 0) {
            cache[idx] = (seq_t)1;
        } else if (i < 4) {
            cache[idx] = (seq_t)0;
        } else {
            seq_t val = (seq_t)0;
            if (i >= 4)  val += cache[(size_t)(i - 4)];
            if (i >= 5)  val += cache[(size_t)(i - 5)];
            if (i >= 6)  val += cache[(size_t)(i - 6)];
            if (i >= 10) val -= cache[(size_t)(i - 10)];
            if (i >= 11) val -= cache[(size_t)(i - 11)];
            if (i >= 13) val -= cache[(size_t)(i - 13)];
            if (i >= 14) val -= cache[(size_t)(i - 14)];
            if (i >= 18) val += cache[(size_t)(i - 18)];
            if (i >= 19) val += cache[(size_t)(i - 19)];
            if (i >= 20) val += cache[(size_t)(i - 20)];
            if (i >= 24) val -= cache[(size_t)(i - 24)];
            cache[idx] = val;
        }
    }
    cache_used = n + 1;
    return cache[(size_t)n];
}
