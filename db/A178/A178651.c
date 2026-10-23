/*
 * A178651  Expansion of the polynomial
 *          (1+x^3)*(1+x^11)*(1+x^15)*(1+x^19)*(1+x^23)*(1+x^27)*(1+x^35).
 * https://oeis.org/A178651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = number of subsets of {3, 11, 15, 19, 23, 27, 35} with sum
 *            n
 * Range:     u8 n <= 133, u16 n <= 133, u32 n <= 133, u64 n <= 133,
 *            u128 n <= 133, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* exponents in the product */
    static const idx_t exps[7] = {3, 11, 15, 19, 23, 27, 35};
    enum { EXPS = 7, DEG = 134 };          /* maximum sum 133 + 1 for 0‑based index */

    static seq_t coeff[DEG] = {0};
    static bool initialized = false;

    if (!initialized) {
        coeff[0] = 1;                        /* empty subset */
        for (size_t k = 0; k < EXPS; ++k) {
            idx_t e = exps[k];
            for (int i = DEG - 1; i >= (int)e; --i) {
                coeff[i] += coeff[i - e];
            }
        }
        initialized = true;
    }

    if (n >= DEG)
        return 0;
    return coeff[(size_t)n];
}
