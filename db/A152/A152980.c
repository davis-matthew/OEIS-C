/*
 * A152980  First differences of toothpick corner sequence A153006.
 * https://oeis.org/A152980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = coefficient of x^n in (1 + x) * ∏_{k≥1} (1 + x^(2^k-1) +
 *            2·x^(2^k))
 * Range:     u8 n <= 121, u16 n <= 4087, u32 n <= 9559, u64 n <= 9559,
 *            u128 n <= 9559, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the product directly, storing the polynomial
   coefficients up to degree n in a dynamic array.  The algorithm
   is O(n log n) and uses only integer arithmetic that the type
   seq_t supports.  The helper functions used are standard
   allocation functions from <stdlib.h>. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The polynomial coefficients up to degree n */
    size_t len = (size_t)n + 1;
    seq_t *coeff = (seq_t *)calloc(len, sizeof(seq_t));
    if (!coeff) return 0;                       /* allocation failure */

    /* Initial factor (1 + x) */
    coeff[0] = 1;
    if (n >= 1) coeff[1] = 1;

    /* Multiply by each factor (1 + x^(2^k-1) + 2·x^(2^k)) */
    for (idx_t m = 2; m - 1 <= n; m <<= 1) {
        seq_t *newcoeff = (seq_t *)calloc(len, sizeof(seq_t));
        if (!newcoeff) {
            free(coeff);
            return 0;
        }

        for (idx_t i = 0; i <= n; ++i) {
            seq_t c = coeff[i];
            if (c == 0) continue;

            /* +1 */
            newcoeff[i] += c;

            /* +x^(m-1) */
            idx_t s1 = i + (m - 1);
            if (s1 <= n) newcoeff[s1] += c;

            /* +2·x^m */
            idx_t s2 = i + m;
            if (s2 <= n) {
                seq_t two_c = c + c;          /* 2 * c */
                newcoeff[s2] += two_c;
            }
        }

        free(coeff);
        coeff = newcoeff;
    }

    seq_t result = coeff[n];
    free(coeff);
    return result;
}
