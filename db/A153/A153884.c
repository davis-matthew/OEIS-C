/*
 * A153884  a(n) = ((7 + sqrt(5))^n - (7 - sqrt(5))^n)/(2*sqrt(5)).
 * https://oeis.org/A153884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((7 + sqrt(5))^n - (7 - sqrt(5))^n)/(2*sqrt(5))
 *            Recurrence: a(1) = 1, a(2) = 14, a(n) = 14*a(n-1) - 44*a(n-2)
 *            for n > 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* allocated capacity */
    static idx_t max_computed = 0; /* highest index computed */

    if (max_computed == 0) { /* first call: initialise */
        cap = 3; /* indices 0,1,2 needed */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* fallback: allocation failed */
        cache[0] = 0; /* not part of OEIS offset, used for recurrence */
        cache[1] = 1;
        cache[2] = 14;
        max_computed = 2;
    }

    /* Grow capacity if needed */
    if (n >= cap) {
        idx_t new_cap = cap * 2;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return cache[n]; /* keep returning last known value on realloc failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = ((seq_t)14 * cache[i - 1]) - ((seq_t)44 * cache[i - 2]);
        }
        max_computed = n;
    }

    return cache[n];
}
