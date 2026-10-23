/*
 * A178692  Expansion of the polynomial
 *          (1+x^3)*(1+x^5)*(1+x^7)*(1+x^9)*(1+x^11)*(1+x^13)*(1+x^15).
 * https://oeis.org/A178692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in
 *            (1+x^3)(1+x^5)(1+x^7)(1+x^9)(1+x^11)(1+x^13)(1+x^15)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static const idx_t exps[7] = {3, 5, 7, 9, 11, 13, 15};
    static seq_t coeff[64];
    static bool initialized = false;
    if (!initialized) {
        for (idx_t i = 0; i < 64; ++i)
            coeff[i] = (seq_t)0;
        coeff[0] = (seq_t)1;
        for (idx_t k = 0; k < 7; ++k) {
            idx_t e = exps[k];
            for (idx_t i = 63; i >= e; --i) {
                coeff[i] = coeff[i] + coeff[i - e];
            }
        }
        initialized = true;
    }
    if (n > 63)
        return (seq_t)0;
    return coeff[n];
}
