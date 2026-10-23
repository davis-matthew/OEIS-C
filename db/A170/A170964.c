/*
 * A170964  Expansion of Product_{i=1..m} (1 + x^(4*i-1)) for m = 11.
 * https://oeis.org/A170964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = coefficient of x^n in ∏_{i=1..11} (1 + x^(4*i-1))
 * Range:     u8 n <= 253, u16 n <= 253, u32 n <= 253, u64 n <= 253,
 *            u128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Product of 11 factors: (1 + x^3)(1 + x^7) … (1 + x^43) */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 254 registers (0 … 253) for the coefficients of the polynomial */
    static seq_t coeffs[254];
    static bool initialized = false;

    if (!initialized) {
        /* clear coefficients (static array is already zeroed, but do it
           explicitly for clarity) */
        for (size_t i = 0; i < 254; ++i)
            coeffs[i] = 0;

        /* enumerate all subsets of the 11 exponents 3,7,11,…,43 */
        const idx_t num_terms = 11;
        const idx_t max_mask = ((idx_t)1) << num_terms;    /* 2048 */

        for (idx_t mask = 0; mask < max_mask; ++mask) {
            idx_t sum = 0;
            for (int i = 0; i < (int)num_terms; ++i) {
                if (mask & (((idx_t)1) << i))
                    sum += (4 * ((idx_t)i + 1) - 1);
            }
            coeffs[(size_t)sum] = coeffs[(size_t)sum] + 1;
        }
        initialized = true;
    }

    if (n < 254)
        return coeffs[(size_t)n];
    else
        return 0;
}
