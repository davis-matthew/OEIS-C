/*
 * A041323  Denominators of continued fraction convergents to sqrt(175).
 * https://oeis.org/A041323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 4048*a(n-6) - a(n-12) (for n ≥ 12) * with initial values:
 *            * a(0)=1, a(1)=4, a(2)=9, a(3)=13, a(4)=35, a(5)=153, *
 *            a(6)=4013, a(7)=16205, a(8)=36423, a(9)=52628, * a(10)=141679,
 *            a(11)=619344
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache dynamically sized to hold a[0..max_n] */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initial 12 terms */
    static const seq_t init[12] = {
        (seq_t)1, (seq_t)4, (seq_t)9, (seq_t)13,
        (seq_t)35, (seq_t)153, (seq_t)4013, (seq_t)16205,
        (seq_t)36423, (seq_t)52628, (seq_t)141679, (seq_t)619344
    };

    /* quick return for the first 12 indices */
    if (n < 12)
        return init[(size_t)n];

    size_t target = (size_t)n;

    /* if cache is not big enough, grow it */
    if (cache_size <= target) {
        size_t new_size = target + 1;

        /* allocate new storage (C++ or C compatible) */
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_size];
#else
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
#endif
        if (!new_cache)
            return (seq_t)0;      /* out‑of‑memory fallback */

        /* copy existing values if any */
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
        }

        /* initialise first 12 entries if this is the first allocation */
        if (!cache) {
            for (size_t i = 0; i < 12; ++i)
                new_cache[i] = init[i];
        }

        /* compute new terms from the point where we stopped */
        size_t start = (cache ? cache_size : 12);
        for (size_t i = start; i <= target; ++i)
            new_cache[i] = (seq_t)4048 * new_cache[i-6] - new_cache[i-12];

        /* free old cache */
        if (cache) {
#ifdef __cplusplus
            delete[] cache;
#else
            free(cache);
#endif
        }

        cache      = new_cache;
        cache_size = new_size;
    }

    return cache[target];
}
