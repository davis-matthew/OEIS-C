/*
 * A156431  Number of n X n arrays of squares of integers, symmetric under
 *          90-degree rotation, with all rows summing to 2.
 * https://oeis.org/A156431
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for odd n; * a(2k) = 2*(k-1)*(k-2)*a(2k-6) -
 *            (k-1)*a(2k-4) + (2k-1)*a(2k-2). * Base values: a(0)=1, a(2)=1,
 *            a(4)=2, a(6)=12
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 35, u128 n <= 57,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;                      /* below offset */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed_up_to = 0;

    /* Ensure the cache can hold indices up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise base values once. */
    if (computed_up_to == 0) {
        ensure_cache(6, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)0;
        cache[6] = (seq_t)12;
        computed_up_to = 6;
    }

    /* Compute values up to the requested n. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if ((i & 1) == 1) {              /* odd indices */
            cache[i] = (seq_t)0;
        } else {                          /* even indices */
            idx_t k = i >> 1;           /* i = 2k */
            seq_t term2k_6 = cache[i - 6];
            seq_t term2k_4 = cache[i - 4];
            seq_t term2k_2 = cache[i - 2];

            seq_t coeff1 = (seq_t)(k - 1);
            coeff1 *= (seq_t)(k - 2);
            coeff1 *= (seq_t)2;

            seq_t term = coeff1 * term2k_6
                        - (seq_t)(k - 1) * term2k_4
                        + (seq_t)(2 * k - 1) * term2k_2;

            cache[i] = term;
        }
    }
    computed_up_to = n;
    return cache[n];
}
