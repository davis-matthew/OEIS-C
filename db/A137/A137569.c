/*
 * A137569  Expansion of f(-x) / f(-x^3) in powers of x where f() is a
 *          Ramanujan theta function.
 * https://oeis.org/A137569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1, k mod 3 ≠ 0} (1 − x^k)
 *            (OEIS A137569)
 * Range:     i8 n <= 53, i16 n <= 155, i32 n <= 500, i64 n <= 1718,
 *            i128 n <= 6227, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *coeff = NULL;   /* a[0 .. current_max] */
    static idx_t current_max = 0; /* largest n for which coeff is valid */

    if (!coeff || n > current_max) {
        /* Allocate a fresh array large enough for the new n. */
        if (coeff)
            free(coeff);
        coeff = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!coeff)
            return (seq_t)0;          /* allocation failure */

        coeff[0] = (seq_t)1;          /* ∏ (1−x^k) starts with 1 */

        /* Multiply by each factor (1 − x^k), k not divisible by 3. */
        for (idx_t k = 1; k <= n; ++k) {
            if ((k % 3) == 0)   /* skip multiples of 3 */
                continue;

            /* Update coefficients in descending order to use the old values. */
            for (idx_t i = n; ; --i) {
                coeff[i] -= coeff[i - k];
                if (i == k)      /* reached the lowest index for this k */
                    break;
            }
        }
        current_max = n;
    }
    return coeff[n];
}
