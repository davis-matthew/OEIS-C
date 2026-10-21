/*
 * A231182  Coefficients for the nonnegative powers of rho(11) = 2*cos(Pi/11)
 *          when written in the power basis of the degree 5 number field
 *          Q(rho(11)). Coefficients for the zeroth and fourth powers.
 * https://oeis.org/A231182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 3*a(n-4) + a(n-5), with
 *            initial values a(0)=1, a(1)=a(2)=a(3)=a(4)=0
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 72, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Initialize cache with first 5 terms if not already done. */
    if (cache == NULL) {
        size_t init_cap = 5;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failure; unlikely in tests */

        cache[0] = 1;          /* a(0) */
        for (size_t i = 1; i < 5; ++i)
            cache[i] = 0;      /* a(1) .. a(4) */

        cache_cap = init_cap;
        cache_len = 5;
    }

    /* Extend storage if required. */
    if ((idx_t)n >= (idx_t)cache_cap) {
        size_t new_cap = (idx_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure; unlikely in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n using the recurrence. */
    if ((idx_t)n >= (idx_t)cache_len) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* a(i) = a(i-1) + 4*a(i-2) - 3*a(i-3) - 3*a(i-4) + a(i-5) */
            seq_t val = cache[i-1] + cache[i-5];
            val += 4 * cache[i-2];
            val -= 3 * cache[i-3];
            val -= 3 * cache[i-4];
            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
