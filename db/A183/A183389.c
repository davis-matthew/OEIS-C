/*
 * A183389  Half the number of n X 5 binary arrays with no element unequal to
 *          a strict majority of its king-move neighbors.
 * https://oeis.org/A183389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) - 4*a(n-3) - 11*a(n-4) - 3*a(n-5) +
 *            4*a(n-6) - 8*a(n-7) - 0*a(n-8) + 10*a(n-9) - 2*a(n-10) - a(n-11)
 *            (valid for n > 12; initial values: a(1)=3, a(2)=3, a(3)=3,
 *            a(4)=8, a(5)=21, a(6)=49, a(7)=136, a(8)=355, a(9)=933,
 *            a(10)=2502, a(11)=6653, a(12)=17823)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 46, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Base values for n = 1 .. 12 (index 0 unused) */
static const seq_t _a_base[13] = {
    0ULL, /* placeholder for index 0 */
    3ULL, 3ULL, 3ULL, 8ULL, 21ULL, 49ULL, 136ULL, 355ULL, 933ULL,
    2502ULL, 6653ULL, 17823ULL
};

seq_t a(idx_t n) {
    /* Handle base cases directly */
    if (n <= 12) {
        return _a_base[(size_t)n];
    }

    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    size_t needed = (size_t)n + 1; /* we use index n directly */

    /* Allocate or grow cache if necessary */
    if (cache == NULL) {
        cache_cap = 64;
        while (cache_cap < needed) cache_cap <<= 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed; return zero as fallback */
        /* Initialize base values in cache */
        for (size_t i = 1; i <= 12 && i < cache_cap; ++i) {
            cache[i] = _a_base[i];
        }
    } else if (cache_cap < needed) {
        size_t new_cap = cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
        /* No need to reinitialize existing values */
    }

    /* Compute terms up to n if not already done */
    for (size_t i = 13; i <= (size_t)n; ++i) {
        if (cache[i]) continue; /* already computed in a previous call */

        /* Compute positive and negative contributions separately to avoid
           underflow with unsigned arithmetic. */
        seq_t pos = 0;
        pos = pos + cache[i-1] * (seq_t)2 + cache[i-2] * (seq_t)5
              + cache[i-6] * (seq_t)4 + cache[i-9] * (seq_t)10;

        seq_t neg = 0;
        neg = neg + cache[i-3] * (seq_t)4 + cache[i-4] * (seq_t)11
              + cache[i-5] * (seq_t)3 + cache[i-7] * (seq_t)8
              + cache[i-10] * (seq_t)2 + cache[i-11];

        cache[i] = pos - neg;
    }

    return cache[(size_t)n];
}
