/*
 * A170967  Expansion of Product_{i=0..m-1} (1 + x^(4*i+1)) for m = 4.
 * https://oeis.org/A170967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in (1 + x^1)(1 + x^5)(1 + x^9)(1 +
 *            x^13)
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Computes the n-th term of OEIS A170967. */
seq_t a(idx_t n)
{
    /* Pre‑computed coefficients for the product of the four factors. */
    static seq_t coeff[29];
    static bool initialized = false;

    if (!initialized) {
        /* Zero the array and set the constant term. */
        for (size_t i = 0; i < 29; ++i)
            coeff[i] = 0;
        coeff[0] = 1;

        /* The exponents in the product. */
        const int exps[4] = {1, 5, 9, 13};

        /* Dynamic convolution: for each exponent, add its contribution
           to all reachable sums. */
        for (int e = 0; e < 4; ++e) {
            int exp = exps[e];
            /* Update in reverse to avoid using a freshly updated value. */
            for (int j = 28 - exp; j >= 0; --j) {
                coeff[j + exp] += coeff[j];
            }
        }
        initialized = true;
    }

    /* Bounds check: sequence is zero beyond the maximum possible exponent. */
    if (n > 28U)   /* cast to unsigned to avoid signed comparison warnings */
        return 0;

    return coeff[(size_t)n];
}
