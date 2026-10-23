/*
 * A041089  Denominators of continued fraction convergents to sqrt(52).
 * https://oeis.org/A041089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 1298*a(n-6) - a(n-12) (n >= 12) with initial values
 *            a(0)=1, a(1)=4, a(2)=5, a(3)=14, a(4)=19, a(5)=90, a(6)=1279,
 *            a(7)=5206, a(8)=6485, a(9)=18176, a(10)=24661, a(11)=116820
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached_len = 0;
    static const seq_t init[12] = {
        (seq_t)1,   (seq_t)4,   (seq_t)5,   (seq_t)14,
        (seq_t)19,  (seq_t)90,  (seq_t)1279, (seq_t)5206,
        (seq_t)6485,(seq_t)18176,(seq_t)24661,(seq_t)116820
    };

    size_t i, n_sz = (size_t)n;

    /* initialise cache with the first 12 terms */
    if (cache == NULL) {
        cache = (seq_t*)malloc(12 * sizeof(seq_t));
        for (i = 0; i < 12; ++i)
            cache[i] = init[i];
        cached_len = 12;
    }

    /* extend cache if required */
    if (n_sz + 1 > cached_len) {
        size_t new_len = n_sz + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) {
            return (seq_t)0;   /* allocation failure */
        }
        cache = tmp;
        for (i = cached_len; i < new_len; ++i) {
            cache[i] = (seq_t)1298 * cache[i-6] - cache[i-12];
        }
        cached_len = new_len;
    }

    return cache[n_sz];
}
