/*
 * A006480  De Bruijn's S(3,n): (3n)!/(n!)^3.
 * https://oeis.org/A006480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) * (3n)*(3n-1)*(3n-2) / n^3, a(0) = 1 (i.e.
 *            (3n)!/(n!)^3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   a006480_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006480_gcd.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        idx_t f1 = 3 * k, f2 = 3 * k - 1, f3 = 3 * k - 2;
        idx_t den = k * k * k;
        idx_t g;
        g = a006480_gcd(f1, den); f1 /= g; den /= g;
        g = a006480_gcd(f2, den); f2 /= g; den /= g;
        g = a006480_gcd(f3, den); f3 /= g; den /= g;
        /* now gcd(f1*f2*f3, den) = 1, so den divides r exactly */
        if (den != 1) {
            seq_t d = (seq_t)den;
            if (d == 0) d = 1; /* only possible after overflow */
            r /= d;
        }
        r *= (seq_t)f1;
        r *= (seq_t)f2;
        r *= (seq_t)f3;
    }
    return r;
}
