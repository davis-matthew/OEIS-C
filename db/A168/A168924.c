/*
 * A168924  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * a(1) = 7 * a(n) = 6*a(n-1) for 2 ≤ n ≤ 21 * a(n) =
 *            5*Σ_{i=1}^{21} a(n-i) – 14*a(n-22) for n ≥ 22 * (The recurrence
 *            follows from the rational generating function given in the OEIS
 *            entry.)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;     /* cache of computed terms */
    static size_t cap   = 0;        /* allocated capacity */
    static idx_t  computed = (idx_t)-1; /* highest index already computed */

    /* Initialise cache on first call */
    if (cache == NULL) {
        /* Allocate space for the first 22 terms (indices 0..21) */
        cap = 22;
        cache = seq_alloc_array(cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        for (idx_t i = 2; i < 22; ++i) {
            cache[i] = cache[i-1] * (seq_t)6;
        }
        computed = 21;
    }

    /* Grow the array if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        cache = seq_realloc(cache, cap, new_cap);
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i >= 22) {
            /* a(i) = 5 * sum_{j=1}^{21} a(i-j) – 14 * a(i-22) */
            seq_t sum = 0;
            for (idx_t j = 1; j <= 21; ++j) {
                sum += cache[i - j];
            }
            sum = sum * (seq_t)5;
            seq_t sub = cache[i - 22] * (seq_t)14;
            cache[i] = sum - sub;
        }
        /* else terms 0..21 are already initialised */
    }
    computed = n;

    return cache[n];
}
