/*
 * A170370  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
 * a(0) = 1
 * a(n) = 23 * Σ_{k=1}^{42} a(n-k) - 276 * a(n-43) + p(n)   (n > 0)
 * where 
 *   p(0) = 1
 *   p(n) = 2 for 1 ≤ n ≤ 42
 *   p(43) = 1
 *   p(n) = 0 for n > 43
 *
 * The coefficients come from the rational generating function
 *   P(t)/Q(t) where
 *   P(t) = t^43 + 2 t^42 + … + 2 t + 1
 *   Q(t) = 276 t^43 - 23 t^42 - … - 23 t + 1
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache (static on the first call).  The cache grows
       as needed.  The implementation uses malloc/realloc for C, and
       new[] / delete[] for C++.  This guarantees construction
       and destruction for an arbitrary‑precision seq_t type. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t max_index = 0;

    /* Initialise cache with a(0) = 1 */
    if (capacity == 0) {
        capacity = 1;
#ifdef __cplusplus
        cache = new seq_t[capacity];
#else
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) exit(1);
#endif
        cache[0] = (seq_t)1;
        max_index = 0;
    }

    /* Return cached value if already computed */
    if (n <= max_index) {
        return cache[n];
    }

    /* Compute terms up to n */
    for (idx_t i = max_index + 1; i <= n; ++i) {
        /* Grow the cache if necessary */
        if ((size_t)i >= capacity) {
            size_t new_capacity = capacity > 0 ? capacity * 2 : 1;
            while (new_capacity <= (size_t)i) new_capacity <<= 1;
#ifdef __cplusplus
            seq_t *tmp = new seq_t[new_capacity];
            for (size_t j = 0; j < capacity; ++j) {
                tmp[j] = cache[j];
            }
            delete[] cache;
            cache = tmp;
#else
            seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
#endif
            capacity = new_capacity;
        }

        /* Sum of the previous up to 42 terms */
        seq_t sum = 0;
        for (idx_t k = 1; k <= 42 && i >= k; ++k) {
            sum += cache[i - k];
        }

        /* Base recurrence: 23 * sum */
        seq_t value = sum * (seq_t)23;

        /* Add numerator coefficient p(i) */
        if (i <= 42) {
            value += (seq_t)2;
        } else if (i == 43) {
            value += (seq_t)1;
        } else {
            /* p(i) = 0 for i > 43 */
        }

        /* Subtract 276 * a(i-43) when i ≥ 43 */
        if (i >= 43) {
            value -= cache[i - 43] * (seq_t)276;
        }

        cache[i] = value;
    }

    max_index = n;
    return cache[n];
}
