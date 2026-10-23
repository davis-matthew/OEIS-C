/*
 * A042055  Denominators of continued fraction convergents to sqrt(551).
 * https://oeis.org/A042055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=17, a(3)=19, a(4)=169, a(5)=357,
 *            a(6)=16591, a(7)=33539, a(8)=284903, a(9)=318442, a(10)=2832439,
 *            a(11)=5983320; for n≥12: a(n) = 16760*a(n-6) - a(n-12)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* The types seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    static const seq_t base_vals[12] = {
        1, 2, 17, 19, 169, 357, 16591, 33539,
        284903, 318442, 2832439, 5983320
    };
    if (n < 12) {
        return base_vals[n];
    }

    /* Static cache to store computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t next_compute = 12; /* next index that needs to be filled */

    /* Allocate cache on first use */
    if (!cache) {
        cache_cap = 12;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 12; ++i) {
            cache[i] = base_vals[i];
        }
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute terms up to n */
    for (idx_t i = next_compute; i <= n; ++i) {
        cache[i] = (seq_t)16760 * cache[i - 6] - cache[i - 12];
    }
    next_compute = n + 1;

    return cache[n];
}
