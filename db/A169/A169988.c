/*
 * A169988  Expansion of Product_{i=0..m-1} (1 + x^(2*i+1)) for m=5.
 * https://oeis.org/A169988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of subsets of {1,3,5,7,9} whose sum is n
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Method: precompute the coefficients of the polynomial
 *  ∏_{i=0..4} (1 + x^(2*i+1)) = (1+x)(1+x^3)(1+x^5)(1+x^7)(1+x^9) .
 *  The resulting coefficients are zero for n > 25 (the maximum sum). */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The polynomial has degree 25, so the largest coefficient index is 25. */
    static seq_t coeff[26];
    static bool initialized = false;

    if (!initialized) {
        /* initialise coefficients to zero */
        for (int i = 0; i < 26; ++i) coeff[i] = 0;

        /* odd exponents in the product */
        const int vals[5] = {1, 3, 5, 7, 9};

        /* enumerate all 2^5 = 32 subsets */
        for (int mask = 0; mask < 32; ++mask) {
            int sum = 0;
            for (int j = 0; j < 5; ++j) {
                if (mask & (1 << j))
                    sum += vals[j];
                /* sum will never exceed 25 */
            }
            /* sum is in [0,25], safe array index */
            coeff[sum] += 1;
        }
        initialized = true;
    }

    /* For n > 25, the coefficient is zero. */
    if (n > 25)
        return 0;

    /* n is now <= 25, safe to index. */
    return coeff[(size_t)n];
}
