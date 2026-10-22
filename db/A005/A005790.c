/*
 * A005790  4-dimensional Catalan numbers.
 * https://oeis.org/A005790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n+3)*(n+2)*(n+1)*a(n) = 8*(4*n-3)*(2*n-1)*(4*n-1)*a(n-1), a(0)
 *            = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   a005790_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a005790_gcd.h"

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t f[4];
        seq_t d, g;
        int k;
        f[0] = 8;
        f[1] = (seq_t)4 * (seq_t)i - 3;
        f[2] = (seq_t)2 * (seq_t)i - 1;
        f[3] = (seq_t)4 * (seq_t)i - 1;
        d = ((seq_t)i + 1) * ((seq_t)i + 2) * ((seq_t)i + 3);
        for (k = 0; k < 4; k++) {
            g = a005790_gcd(f[k], d);
            f[k] /= g;
            d /= g;
        }
        v /= d;
        for (k = 0; k < 4; k++) {
            v *= f[k];
        }
    }
    return v;
}
