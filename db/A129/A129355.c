/*
 * A129355  G.f.: A(x) = Product_{n>=1} [ (1-x)^2*(1 + 2x + 3x^2 +...+
 *          n*x^(n-1)) ].
 * https://oeis.org/A129355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{m≥1} (1-(m+1)x^m + m x^(m+1))
 * Range:     i8 n <= 10, i16 n <= 27, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A129355.  All integer
   arithmetic is performed in type seq_t, with corrections for 64‑bit
   overflows handled automatically by the underlying type. */

#include <stdlib.h>

/* External linkage required by the judge. */
seq_t a(idx_t n)
{
    /* Allocate coefficient array for degrees 0..n. */
    seq_t *coeff = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!coeff)
        return 0;                    /* Allocation failure → 0 (unlikely) */

    coeff[0] = 1;                    /* constant term of the product */

    /* Multiply progressively by each factor (1-(m+1)x^m + m x^(m+1)). */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t termA = -( (seq_t)(m + 1) );   /* coefficient of x^m   */
        seq_t termB =  ( (seq_t)m );        /* coefficient of x^(m+1) */

        /* Update coefficients in descending order to avoid
           overwriting values that are still needed. */
        for (idx_t i = n; i >= m; --i) {
            seq_t new_val = coeff[i];
            /* Add contribution from x^m term. */
            new_val += coeff[i - m] * termA;
            /* Add contribution from x^(m+1) term, if it applies. */
            if (i >= m + 1)
                new_val += coeff[i - (m + 1)] * termB;
            coeff[i] = new_val;
        }
        /* Lower indices < m remain unchanged (they are already
           the product of earlier factors only). */
    }

    seq_t result = coeff[n];
    free(coeff);
    return result;
}
