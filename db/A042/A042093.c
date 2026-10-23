/*
 * A042093  Denominators of continued fraction convergents to sqrt(570).
 * https://oeis.org/A042093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 382*a(n-4) - a(n-8), for n>7 a(0)=1, a(1)=1, a(2)=7,
 *            a(3)=8, a(4)=375, a(5)=383, a(6)=2673, a(7)=3056
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* persistent cache */
    static seq_t *cache     = NULL;   /* pointer to the array */
    static size_t cache_len = 0;      /* highest initialized index + 1 */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* ensure we can address cache[n] */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);   /* memory allocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* base values */
    static const seq_t init[8] = {
        (seq_t)1,  (seq_t)1,  (seq_t)7,  (seq_t)8,
        (seq_t)375,(seq_t)383,(seq_t)2673,(seq_t)3056
    };

    /* if we already have a cached answer, return it */
    if (n < cache_len) return cache[n];

    /* compute all missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 8) {
            cache[i] = init[i];
        } else {
            cache[i] = ((seq_t)382) * cache[i-4] - cache[i-8];
        }
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
