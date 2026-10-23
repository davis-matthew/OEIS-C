/*
 * A041075  Denominators of continued fraction convergents to sqrt(44).
 * https://oeis.org/A041075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 398*a(n-8) - a(n-16) Initial terms: a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=8, a(5)=11, a(6)=19, a(7)=30, a(8)=379,
 *            a(9)=409, a(10)=788, a(11)=1197, a(12)=3182, a(13)=4379,
 *            a(14)=7561, a(15)=11940
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 61, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements currently computed (index+1) */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* Seed values for the recurrence */
    static const seq_t init[16] = {
        (seq_t)1,   (seq_t)1,   (seq_t)2,   (seq_t)3,
        (seq_t)8,   (seq_t)11,  (seq_t)19,  (seq_t)30,
        (seq_t)379, (seq_t)409, (seq_t)788, (seq_t)1197,
        (seq_t)3182,(seq_t)4379,(seq_t)7561,(seq_t)11940
    };

    /* Ensure the cache can hold the value at index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 32)
            new_cap = 32;

        seq_t *tmp;
        if (cache == NULL) {
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (!tmp)          /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to index n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t val;
        if (i < 16) {       /* use seeded values */
            val = init[i];
        } else {            /* recurrence */
            val = ((seq_t)398 * cache[i - 8]) - cache[i - 16];
        }
        cache[i] = val;
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
