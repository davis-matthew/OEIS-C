/*
 * A170979  Expansion of (Product_{i=1..m-1} (1 + x^(4*i-1))) * (1 +
 *          x^(2*m-1)) for m = 7.
 * https://oeis.org/A170979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of subsets of {3,7,11,15,19,23,13} whose exponents
 *            sum to n
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Max exponent sum = 3+7+11+15+19+23+13 = 91, so array size 92. */
    static seq_t coeffs[92];
    static int initialized = 0;
    if (!initialized) {
        static const idx_t exponents[7] = {3, 7, 11, 15, 19, 23, 13};
        /* Enumerate all 2^7 subsets and count sums. */
        for (int mask = 0; mask < (1 << 7); ++mask) {
            idx_t sum = 0;
            for (int i = 0; i < 7; ++i) {
                if (mask & (1 << i))
                    sum += exponents[i];
            }
            coeffs[(size_t)sum] = coeffs[(size_t)sum] + (seq_t)1;
        }
        initialized = 1;
    }
    if (n < 92)
        return coeffs[(size_t)n];
    return (seq_t)0;
}
