/*
 * A000777  a(n) = (n+2)*Catalan(n) - 1.
 * https://oeis.org/A000777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*Catalan(n) - 1, with Catalan(k+1) =
 *            Catalan(k)*2*(2k+1)/(k+2), Catalan(0) = 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   a000777_gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000777_gcd_idx.h"

seq_t a(idx_t n) {
    seq_t c = 1;
    idx_t k;
    for (k = 0; k < n; k++) {
        idx_t num = 2 * (2 * k + 1);
        idx_t den = k + 2;
        idx_t g = a000777_gcd_idx(num, den);
        num /= g;
        den /= g;
        /* den divides c exactly since gcd(num, den) = 1 and the result is integral */
        c /= (seq_t)den;
        c *= (seq_t)num;
    }
    return ((seq_t)n + 2) * c - 1;
}
