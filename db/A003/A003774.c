/*
 * A003774  Number of spanning trees with degrees 1 and 3 in K_4 X P_n.
 * https://oeis.org/A003774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: 4*x*(1+20*x^2+12*x^3+48*x^5+24*x^6)/(1-12*x-4*x^2-48*x^3),
 *            i.e. a(n) = 4*c(n-1) where c(k) = p(k) + 12*c(k-1) + 4*c(k-2) +
 *            48*c(k-3), c(k) = 0 for k < 0, and p = 1,0,20,12,0,48,24 (then
 *            0) is the numerator; so a(n) = 12*a(n-1) + 4*a(n-2) + 48*a(n-3)
 *            for n > 7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   num_coef
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/num_coef.h"

seq_t a(idx_t n) {
    seq_t c1 = 0, c2 = 0, c3 = 0; /* c(k-1), c(k-2), c(k-3) */
    seq_t c = 0;
    idx_t k;
    if (n < 1) return 0;
    for (k = 0; k < n; k++) {
        c = c1 * 12 + c2 * 4 + c3 * 48;
        c += num_coef(k);
        c3 = c2;
        c2 = c1;
        c1 = c;
    }
    return c * 4;
}
