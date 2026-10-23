/*
 * A117636  Start with x=4/3; repeatedly apply the map x -> x ceiling(x^2);
 *          sequence gives numerators of the resulting sequence of fractions.
 * https://oeis.org/A117636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117636:
   a(1)=4 and for n>1
   let x_{n-1}=a(n-1)/d_{n-1} with d_{1}=3.
   x_n = x_{n-1} * ceil(x_{n-1}^2)  (ceil on integer)
   a(n) is the numerator of the reduced fraction x_n.
   The denominator after each step is automatically simplified
   by taking gcd(x_{n-1},denominator). */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n<1; return 0 */

    /* static cache of already computed terms */
    static seq_t *cache = NULL;   /* index from 1 up to cache_len */
    static size_t cache_len = 0;

    /* state of the current fraction (numerator and denominator) */
    static seq_t last_num = 0;
    static seq_t last_den = 1;

    /* initialise on the first call */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_len + 1));
        cache[1] = (seq_t)4;      /* a(1) */
        last_num = (seq_t)4;
        last_den = (seq_t)3;      /* x_1 = 4/3 */
    }

    /* return from cache if possible */
    if ((size_t)n <= cache_len)
        return cache[n];

    /* grow cache to hold up to term n */
    size_t new_len = (size_t)n;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_len + 1));
    for (size_t i = cache_len + 1; i <= new_len; ++i) {
        /* ceil( (last_num / last_den)^2 ) */
        seq_t num_sq = last_num * last_num;
        seq_t den_sq = last_den * last_den;
        seq_t m = (num_sq + den_sq - 1) / den_sq;   /* ceil division */

        /* multiply and reduce fraction */
        last_num *= m;
        seq_t g = gcd(last_num, last_den);
        last_num /= g;
        last_den /= g;

        cache[i] = last_num;       /* numerator is a(i) */
    }

    cache_len = new_len;
    return cache[n];
}
