/*
 * A073851  Cumulative sum of initial digits of (n base 5).
 * https://oeis.org/A073851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = a(n-1) + first_digit_base5(n), a(0) = 0
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   first_digit_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* first_digit_base5(n) is the most significant digit of n in base 5. */

#include <stdlib.h>
#include "../utils/first_digit_base5.h"

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of terms currently stored (>=1) */
    static size_t capacity = 0;

    /* initialise cache on first call */
    if (cached == 0) {
        capacity = 16;                     /* arbitrary small start */
        cache = (seq_t*)realloc(NULL, capacity * sizeof(seq_t));
        if (!cache) return 0;              /* fail-safe: unreachable in tests */
        cache[0] = 0;                      /* a(0) = 0 */
        cached = 1;
    }

    /* enlarge cache if necessary */
    if ((idx_t)n >= capacity) {
        size_t new_cap = capacity;
        while ((idx_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* fail-safe */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing terms up to n */
    if ((idx_t)n >= cached) {
        for (idx_t i = cached; i <= n; ++i) {
            seq_t fd = first_digit_base5(i);
            cache[i] = cache[i - 1] + fd;
        }
        cached = n + 1;
    }

    return cache[n];
}
