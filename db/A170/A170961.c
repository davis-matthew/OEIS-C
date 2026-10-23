/*
 * A170961  Expansion of Product_{i=1..m} (1 + x^(4*i-1)) for m = 8.
 * https://oeis.org/A170961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = coefficient of x^n in ∏_{i=1..8} (1 + x^(4*i-1))
 * Range:     u8 n <= 136, u16 n <= 136, u32 n <= 136, u64 n <= 136,
 *            u128 n <= 136, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t coeff[137];
    static bool ready = false;
    if (!ready) {
        /* initialize coefficients of the product (1 + x^e) for e = 3,7,...,31 */
        const idx_t exps[8] = {3, 7, 11, 15, 19, 23, 27, 31};
        const idx_t maxdeg = 136;   /* sum of exponents */
        for (idx_t i = 0; i <= maxdeg; ++i) coeff[i] = 0;
        coeff[0] = 1;
        for (int ei = 0; ei < 8; ++ei) {
            idx_t e = exps[ei];
            for (idx_t k = maxdeg; k >= e; --k) {
                coeff[k] += coeff[k - e];
            }
        }
        ready = true;
    }
    if (n > 136) return 0;
    return coeff[n];
}
