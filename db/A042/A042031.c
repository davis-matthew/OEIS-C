/*
 * A042031  Denominators of continued fraction convergents to sqrt(539).
 * https://oeis.org/A042031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 7940*a(n-8) - a(n-16) with a(0)=1, a(1)=4, a(2)=5,
 *            a(3)=9, a(4)=14, a(5)=23, a(6)=37, a(7)=171, a(8)=7903,
 *            a(9)=31783, a(10)=39686, a(11)=71469, a(12)=111155,
 *            a(13)=182624, a(14)=293779, a(15)=1357740
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 22, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init[16] = {
    (seq_t)1, (seq_t)4, (seq_t)5, (seq_t)9, (seq_t)14, (seq_t)23,
    (seq_t)37, (seq_t)171, (seq_t)7903, (seq_t)31783,
    (seq_t)39686, (seq_t)71469, (seq_t)111155, (seq_t)182624,
    (seq_t)293779, (seq_t)1357740
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache;

#ifdef __cplusplus
        /* Allocate new array and copy existing data */
        new_cache = new seq_t[new_len];
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        delete [] cache;
#else
        /* Reallocate memory in C */
        new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* In C++, realloc returns void*, but we cast explicitly below */
#endif
        if (!new_cache) {
            /* Allocation failed; return 0 as a graceful fallback */
            return (seq_t)0;
        }

        /* Compute missing terms */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i < 16) {
                new_cache[i] = init[(idx_t)i];
            } else {
                new_cache[i] = (seq_t)7940 * new_cache[i-8] - new_cache[i-16];
            }
        }

        cache = new_cache;
        cache_len = new_len;
    }
    return cache[n];
}
