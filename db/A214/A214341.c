/*
 * A214341  Expansion of 1 / k(q) = 1 / (r(q) * r(q^2)^2) in powers of q where
 *          r() is the Rogers-Ramanujan continued fraction.
 * https://oeis.org/A214341
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 49, i16 n <= 145, i32 n <= 460, i64 n <= 1585,
 *            i128 n <= 5755, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A214341
 *  Coefficients of the eta‑quotient
 *
 *  G.f. = (1/x) * ∏_{k≥1}
 *         (1 – x^(10k-3)) * (1 – x^(10k-4)) * (1 – x^(10k-6)) * (1 – x^(10k-7))
 *         /((1 – x^(10k-1)) * (1 – x^(10k-2)) * (1 – x^(10k-8)) * (1 – x^(10k-9)))
 *
 *  a(n) = coefficient of x^(n+1) in the product above.
 *  The function recomputes the required coefficients from scratch
 *  for each call (O(n²) work, n ≤ 10 000).  This is adequate for the
 *  specified time limits.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ –1.  For smaller values return 0. */
    if (n < -1)
        return (seq_t)0;

    /* Number of terms needed in the array: indices 0 … n+1. */
    size_t N = (size_t)(n + 2);      /* n = –1 ⇒ N = 1; n ≥ 0 ⇒ N ≥ 2 */

    /* Allocate the coefficient array, initialized to 0. */
    seq_t *coeff = (seq_t *)calloc(N, sizeof(seq_t));
    if (!coeff)
        exit(EXIT_FAILURE);        /* allocation failed */

    coeff[0] = (seq_t)1;           /* constant term of the infinite product */

    /* Multiply the product factor by factor.  The factor for exponent i
     * is (1 – x^i) for i ≡ 3,4,6,7  (mod 10)     (numerator)
     * and 1/(1 – x^i) for i ≡ 1,2,8,9  (mod 10) (denominator).
     * All other residues give a factor of 1.              */
    for (size_t i = 1; i < N; ++i) {
        size_t r = i % 10;

        if (r == 1 || r == 2 || r == 8 || r == 9) { /* denominator 1/(1‑x^i) */
            for (size_t j = i; j < N; ++j)
                coeff[j] += coeff[j - i];
        }
        else if (r == 3 || r == 4 || r == 6 || r == 7) { /* numerator (1‑x^i) */
            /* iterate j from N-1 down to i to avoid using newly updated
             * values in the same step; a typical trick for reversed loops
             * with unsigned indices.                                        */
            for (size_t j = N; j-- > i; )
                coeff[j] -= coeff[j - i];
        }
        /* residues 0 and 5 are skipped (factor = 1) */
    }

    seq_t result = coeff[(size_t)(n + 1)];
    free(coeff);
    return result;
}
