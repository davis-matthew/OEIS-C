/*
 * A170962  Expansion of Product_{i=1..m} (1 + x^(4*i-1)) for m = 9.
 * https://oeis.org/A170962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = coefficient of x^n in ∏_{i=1..9} (1 + x^(4*i-1))
 * Range:     u8 n <= 171, u16 n <= 171, u32 n <= 171, u64 n <= 171,
 *            u128 n <= 171, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_SUM 171

seq_t a(idx_t n)
{
    static seq_t cache[MAX_SUM + 1];
    static int initialized = 0;

    if (!initialized) {
        /* initialise DP for all subset–sum coefficients up to MAX_SUM */
        cache[0] = 1;
        for (idx_t i = 0; i < 9; ++i) {
            int k = (int)((i + 1) * 4 - 1);    /* exponent value (3,7,11,…,35) */
            for (int s = MAX_SUM; s >= k; --s) {
                cache[s] += cache[s - k];
            }
        }
        initialized = 1;
    }

    return (n <= (idx_t)MAX_SUM) ? cache[(int)n] : 0;
}
