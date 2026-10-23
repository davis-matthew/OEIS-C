/*
 * A042139  Denominators of continued fraction convergents to sqrt(594).
 * https://oeis.org/A042139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2196610*a(n-10) - a(n-20) for n>19, a(0)=1, a(1)=2,
 *            a(2)=3, a(3)=8, a(4)=43, a(5)=1040, a(6)=5243, a(7)=11526,
 *            a(8)=16769, a(9)=45064, a(10)=2179841, a(11)=4404746,
 *            a(12)=6584587, a(13)=17573920, a(14)=94454187, a(15)=2284474408,
 *            a(16)=11516826227, a(17)=25318126862, a(18)=36834953089, a(1...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial 20 terms */
    static const seq_t init[20] = {
        (seq_t)1ULL,  (seq_t)2ULL,  (seq_t)3ULL,  (seq_t)8ULL,
        (seq_t)43ULL, (seq_t)1040ULL, (seq_t)5243ULL,
        (seq_t)11526ULL, (seq_t)16769ULL, (seq_t)45064ULL,
        (seq_t)2179841ULL, (seq_t)4404746ULL, (seq_t)6584587ULL,
        (seq_t)17573920ULL, (seq_t)94454187ULL, (seq_t)2284474408ULL,
        (seq_t)11516826227ULL, (seq_t)25318126862ULL,
        (seq_t)36834953089ULL, (seq_t)98988033040ULL
    };
    /* Cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of allocated elements */
    static size_t last_computed = (size_t)-1; /* index of last known value */

    /* If n is among the initial terms, return it directly */
    if (n < 20) {
        return init[(size_t)n];
    }

    /* Allocate initial storage if needed */
    if (cache == NULL) {
        cache_size = 20; /* at least enough for the init terms */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* out of memory – shouldn't happen */
        for (size_t i = 0; i < 20; ++i) cache[i] = init[i];
        last_computed = 19;
    }

    /* Grow cache if it does not cover requested index */
    if (n >= cache_size) {
        size_t new_size = cache_size;
        while (new_size <= n) new_size <<= 1; /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* out of memory – shouldn't happen */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    if (n > last_computed) {
        for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)2196610 * cache[i - 10] - cache[i - 20];
        }
        last_computed = (size_t)n;
    }

    return cache[(size_t)n];
}
