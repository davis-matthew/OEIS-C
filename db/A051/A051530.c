/*
 * A051530  Molien series for group H_{1,3} of order 1152.
 * https://oeis.org/A051530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-5)+
 *            2*a(n-6)-a(n-7)-a(n-8)+a(n-10)+a(n-11)-a(n-12) initial values:
 *            a(0)=1,a(1)=0,a(2)=1,a(3)=3,a(4)=4,a(5)=5,a(6)=15,a(7)=14,
 *            a(8)=24,a(9)=35,a(10)=44,a(11)=54
 * Range:     u8 n <= 18, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache       = NULL;
    static size_t cache_cap   = 0;                  /* allocated size */
    static size_t last_computed = (size_t)-1;       /* highest index computed (inclusive) */

    /* initial terms for n = 0..11 */
    static const seq_t init[12] = {
        1, 0, 1, 3, 4, 5, 15, 14, 24, 35, 44, 54
    };

    /* quick return for the first 12 terms */
    if (n < 12) {
        return init[(size_t)n];
    }

    /* allocate and seed the cache on first use */
    if (!cache) {
        cache_cap = 12;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure */
        for (size_t i = 0; i < 12; ++i) {
            cache[i] = init[i];
        }
        last_computed = 11;
    }

    /* ensure we have enough room for n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all terms up to n if not already done */
    size_t start = last_computed + 1;
    if (start < 12) start = 12;          /* safety: indices <12 already known */
    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2] + cache[i - 10] + cache[i - 11];
        sum += cache[i - 6];            /* +a[i-6] */
        sum += cache[i - 6];            /* +a[i-6] again => +2*a[i-6] */
        seq_t val = sum
                    - cache[i - 4]
                    - cache[i - 5]
                    - cache[i - 7]
                    - cache[i - 8]
                    - cache[i - 12];
        cache[i] = val;
    }
    last_computed = (size_t)n;

    return cache[(size_t)n];
}
