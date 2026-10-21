/*
 * A204770  Expansion of psi(x^3) * f(-x) in powers of x where psi(), f() are
 *          Ramanujan theta functions.
 * https://oeis.org/A204770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A204770: a(n) = coefficient of x^n in 
 *  Product_{k>=1} (1 - x^k)*(1 + x^(3k))*(1 - x^(6k))
 *  The product expands to the generating function listed for the sequence.
 *  The function computes the coefficient by dynamic programming,
 *  iteratively applying each factor for k = 1 … n.
 *  Complexity:  O(n^2), which is easily below 2 s for n ≤ 10⁴.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build system */

seq_t a(idx_t n)
{
    /* Guard against enormous n that would overflow the number of array elements */
    if (n > SIZE_MAX - 1) {
        /* Not expected for the test data; abort safely */
        abort();
    }

    size_t N = (size_t)n;          /* use size_t for array lengths */
    /* Allocate array to store current coefficients up to degree N */
    seq_t *coeff = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!coeff) abort();

    /* Initialise all coefficients to 0; coeff[0] will be set to 1 next */
    for (size_t i = 0; i <= N; ++i) coeff[i] = 0;
    coeff[0] = 1;

    /* Iterate over k and apply the three factors of the product */
    for (size_t k = 1; k <= N; ++k) {
        /* Factor (1 - x^k)  */
        for (size_t j = N; j >= k; --j) coeff[j] -= coeff[j - k];

        /* Factor (1 + x^(3k))  */
        size_t shift3 = 3 * k;
        if (shift3 <= N) {
            for (size_t j = N; j >= shift3; --j) coeff[j] += coeff[j - shift3];
        }

        /* Factor (1 - x^(6k))  */
        size_t shift6 = 6 * k;
        if (shift6 <= N) {
            for (size_t j = N; j >= shift6; --j) coeff[j] -= coeff[j - shift6];
        }
    }

    seq_t result = coeff[N];
    free(coeff);
    return result;
}
