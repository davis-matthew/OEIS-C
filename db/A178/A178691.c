/*
 * A178691  Expansion of the polynomial
 *          (1+x^3)*(1+x^5)*(1+x^7)*(1+x^9)*(1+x^11)*(1+x^13).
 * https://oeis.org/A178691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in
 *            (1+x^3)(1+x^5)(1+x^7)(1+x^9)(1+x^11)(1+x^13)
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Polynomial degree is 48, coefficients are 0 for n>48 */
    if (n > 48ULL)
        return 0;

    static seq_t coeff[49];
    static bool initialized = false;

    if (!initialized) {
        /* initialize all coefficients to 0 */
        for (int i = 0; i < 49; ++i)
            coeff[i] = 0;
        coeff[0] = 1;  /* product starts as 1 */

        /* exponents in the factors (1 + x^e) */
        static const idx_t exps[6] = {3, 5, 7, 9, 11, 13};

        for (int ei = 0; ei < 6; ++ei) {
            int e = (int)exps[ei];
            for (int j = 48 - e; j >= 0; --j) {
                coeff[j + e] += coeff[j];
            }
        }
        initialized = true;
    }

    return coeff[(int)n];
}
