/*
 * A200184  Number of -n..n arrays x(0..5) of 6 elements with zero sum and no
 *          two consecutive declines, no adjacent equal elements, and no
 *          element more than one greater than the previous (random base
 *          sawtooth pattern).
 * https://oeis.org/A200184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-7)+a(n-9)+a(n-10)-a(n-11) for
 *            n>12; base cases: a(1)=6, a(2)=15, a(3)=29, a(4)=48, a(5)=72,
 *            a(6)=103, a(7)=141, a(8)=186, a(9)=244, a(10)=309, a(11)=385,
 *            a(12)=472
 * Range:     u8 n <= 9, u16 n <= 75, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t size = 0; /* largest index computed (0 if none) */

    /* Base values, index 0 unused */
    static const seq_t base_vals[13] = {
        0, 6,15,29,48,72,103,141,186,244,309,385,472
    };

    if (n < 1) return 0; /* undefined below offset, return 0 */

    /* initialise cache */
    if (!cache) {
        cap = 32;                     /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;         /* allocation failed */
        /* fill base values */
        for (size_t i = 1; i <= 12; ++i) cache[i] = base_vals[i];
        for (size_t i = 13; i < cap; ++i) cache[i] = 0;
        size = 12;
    }

    /* extend cache if needed */
    if ((size_t)n > size) {
        size_t needed = (size_t)n + 1;
        if (needed > cap) {
            size_t new_cap = cap;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;       /* allocation failed */
            cache = tmp;
            cap = new_cap;
        }
        /* compute recurrence for new indices */
        for (size_t i = size + 1; i <= (size_t)n; ++i) {
            cache[i] =
                (cache[i-1] + cache[i-2] + cache[i-9] + cache[i-10]) -
                (cache[i-4] + cache[i-7] + cache[i-11]);
        }
        size = (size_t)n;
    }
    return cache[n];
}
