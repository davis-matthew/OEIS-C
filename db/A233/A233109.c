/*
 * A233109  Number of nX4 0..5 arrays with no element x(i,j) adjacent to value
 *          5-x(i,j) horizontally or vertically, top left element zero, and 1
 *          appearing before 2 3 and 4, and 2 appearing before 3 in row major
 *          order.
 * https://oeis.org/A233109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 445*a(n-1) -29415*a(n-2) + 684239*a(n-3) -6544703*a(n-4)
 *            + 26009211*a(n-5) -37153497*a(n-6) +17033721*a(n-7); Initial
 *            values: a(1)=23, a(2)=6046, a(3)=2154065, a(4)=794723482,
 *            a(5)=294285758398, a(6)=109015274761891, a(7)=40385221182281560
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Sequence generator ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined for n<1 */

    /* First seven values are hard‑coded constants. */
    static const seq_t init[7] = {
        23ULL,
        6046ULL,
        2154065ULL,
        794723482ULL,
        294285758398ULL,
        109015274761891ULL,
        40385221182281560ULL
    };

    if (n <= 7) return init[n-1];

    /* Dynamic cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cnt = 0;    /* number of elements stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Lazy initialisation */
    if (!cache) {
        cache_cap = (size_t)n > 7 ? (size_t)n : 7;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */

        /* load the first seven terms */
        for (idx_t i = 0; i < 7 && i < cache_cap; ++i)
            cache[i] = init[i];
        cache_cnt = 7;
    }

    /* Grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 7;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return already computed value */
    if (n <= cache_cnt) return cache[n-1];

    /* Compute from cache_cnt to n-1 (0‑based indices) */
    for (idx_t i = cache_cnt; i < n; ++i) {
        /* Positive contributions: a(i)*coeff for odd offsets */
        seq_t pos = cache[i-1];
        pos *= 445u;
        pos += cache[i-3] * 684239u;
        pos += cache[i-5] * 26009211u;
        pos += cache[i-7] * 17033721u;

        /* Negative contributions: a(i)*coeff for even offsets */
        seq_t neg = cache[i-2];
        neg *= 29415u;
        neg += cache[i-4] * 6544703u;
        neg += cache[i-6] * 37153497u;

        /* Final result: pos - neg (pos >= neg for all n) */
        cache[i] = pos - neg;
    }

    cache_cnt = n;
    return cache[n-1];
}
