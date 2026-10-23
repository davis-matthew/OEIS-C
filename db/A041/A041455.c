/*
 * A041455  Denominators of continued fraction convergents to sqrt(243).
 * https://oeis.org/A041455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 140452*a(n-10) – a(n-20) for n > 19 Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=17, a(5)=260, a(6)=797,
 *            a(7)=1854, a(8)=2651, a(9)=4505, a(10)=137801, a(11)=142306,
 *            a(12)=280107, a(13)=702520, a(14)=2387667, a(15)=36517525,
 *            a(16)=111940242, a(17)=260398009, a(18)=372338251, a(19)=...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init[20] = {
    1ULL, 1ULL, 2ULL, 5ULL, 17ULL,
    260ULL, 797ULL, 1854ULL, 2651ULL, 4505ULL,
    137801ULL, 142306ULL, 280107ULL, 702520ULL,
    2387667ULL, 36517525ULL, 111940242ULL,
    260398009ULL, 372338251ULL, 632736260ULL
};

seq_t a(idx_t n)
{
    /* use a few-compute cache; thread‑safety is not required */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of values already stored */

    /* short‑circuit for the first 20 terms */
    if (n < 20)
        return init[(size_t)n];

    /* initialise the cache if necessary */
    if (!cache) {
        cache_len = 20;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return init[0];      /* allocation failure – give a default value */
        for (size_t i = 0; i < 20; ++i)
            cache[i] = init[i];
    }

    /* grow the cache if we need more space */
    size_t needed = (size_t)n + 1;
    if (needed > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return cache[cache_len - 1];  /* out of memory: return last known */
        cache = tmp;
        cache_len = needed;
    }

    /* compute any new terms that are not yet cached */
    for (size_t i = 20; i <= (size_t)n; ++i) {
        /* use 128‑bit arithmetic to avoid overflow of the product when seq_t is 64‑bit */
        cache[i] = (seq_t)(
            ((__uint128_t)140452 * cache[i - 10]) - cache[i - 20]
        );
    }

    return cache[(size_t)n];
}
