/*
 * A145284  a(n) = number of monomials in n-th power of polynomial
 *          x^3-x^2-x-1.
 * https://oeis.org/A145284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n + 1 for all n >= 15 For n < 15 the values are
 *            computed by explicit convolution. This covers all terms and is
 *            fast for arbitrarily large n
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle large n by the closed form */
    if (n >= 15) {
        return (seq_t)n * 3 + 1;
    }

    /* For n < 15 we perform exact convolution with signed 64‑bit
       arithmetic (the values are guaranteed to fit). */
    const int base_coef[4] = { -1, -1, -1, 1 }; /* coefficients of x^3-x^2-x-1 */
    /* Maximum exponent for n<15 is 3*14 = 42, so 45 slots are enough. */
    long long coeff[45] = {0};

    /* Initialize for n = 1 */
    for (int i = 0; i <= 3; ++i) {
        coeff[i] = base_coef[i];
    }
    int max_exp = 3;

    /* Convolve iteratively up to the requested n */
    for (int iter = 2; iter <= (int)n; ++iter) {
        int new_max = 3 * iter;
        long long newcoeff[45] = {0};

        for (int e = 0; e <= max_exp; ++e) {
            long long val = coeff[e];
            newcoeff[e]     += -val; /* multiply by -1 (x^0 term) */
            newcoeff[e + 1] += -val; /* multiply by -1 (x^1 term) */
            newcoeff[e + 2] += -val; /* multiply by -1 (x^2 term) */
            newcoeff[e + 3] +=  val; /* multiply by +1 (x^3 term) */
        }
        /* copy back to coeff */
        for (int i = 0; i <= new_max; ++i) {
            coeff[i] = newcoeff[i];
        }
        max_exp = new_max;
    }

    /* Count non‑zero coefficients */
    int count = 0;
    for (int i = 0; i <= max_exp; ++i) {
        if (coeff[i] != 0) {
            ++count;
        }
    }
    return (seq_t)count;
}
