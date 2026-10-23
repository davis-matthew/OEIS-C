/*
 * A178687  Expansion of the polynomial
 *          (1+x^3)*(1+x^5)*(1+x^7)*(1+x^9)*(1+x^11).
 * https://oeis.org/A178687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in
 *            (1+x^3)(1+x^5)(1+x^7)(1+x^9)(1+x^11)
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_DEGREE 35

seq_t a(idx_t n) {
    static seq_t coeff[MAX_DEGREE + 1];
    static bool initialized = false;
    if (!initialized) {
        for (idx_t i = 0; i <= MAX_DEGREE; ++i) coeff[i] = 0;
        coeff[0] = 1;
        static const idx_t exponents[5] = {3,5,7,9,11};
        for (size_t k = 0; k < 5; ++k) {
            idx_t e = exponents[k];
            for (idx_t i = MAX_DEGREE; i >= e; --i) {
                coeff[i] += coeff[i - e];
            }
        }
        initialized = true;
    }
    if (n > MAX_DEGREE) return 0;
    return coeff[(size_t)n];
}
