/*
 * A042905  Denominators of continued fraction convergents to sqrt(984).
 * https://oeis.org/A042905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 177610*a(n-10) - a(n-20) for n > 19 Initial terms:
 *            a(0)=1, a(1)=2, a(2)=3, a(3)=8, a(4)=19, a(5)=141, a(6)=301,
 *            a(7)=743, a(8)=1044, a(9)=2831, a(10)=176566, a(11)=355963,
 *            a(12)=532529, a(13)=1421021, a(14)=3374571, a(15)=25043018,
 *            a(16)=53460607, a(17)=131964232, a(18)=185424839, a(19)=50...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for arbitrary‑order calls */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* allocated capacity (number of elements)   */
static idx_t cache_size = 0;  /* number of valid elements in the cache    */

static const idx_t init_len = 20;
static const seq_t init_terms[20] = {
    (seq_t)1,      (seq_t)2,      (seq_t)3,      (seq_t)8,
    (seq_t)19,     (seq_t)141,    (seq_t)301,    (seq_t)743,
    (seq_t)1044,   (seq_t)2831,   (seq_t)176566, (seq_t)355963,
    (seq_t)532529, (seq_t)1421021, (seq_t)3374571,(seq_t)25043018,
    (seq_t)53460607,(seq_t)131964232,(seq_t)185424839,(seq_t)502813910
};

seq_t a(idx_t n)
{
    /* return readily available initial values */
    if (n < init_len)
        return init_terms[n];

    /* initialise cache on first call or grow if necessary */
    if (cache == NULL) {
        cache_cap = n + 1;
        if (cache_cap < 32)          /* protect against tiny n */
            cache_cap = 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) { abort(); }
        for (idx_t i = 0; i < init_len; ++i)
            cache[i] = init_terms[i];
        cache_size = init_len;
    } else if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { abort(); }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return cached value if we have computed it */
    if (n < cache_size)
        return cache[n];

    /* recurrence: a(n) = 177610*a(n-10) - a(n-20) */
    const seq_t coeff = (seq_t)177610;
    for (idx_t i = cache_size; i <= n; ++i)
        cache[i] = coeff * cache[i - 10] - cache[i - 20];

    cache_size = n + 1;
    return cache[n];
}
