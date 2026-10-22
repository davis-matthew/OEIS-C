/*
 * A005746  Number of n-covers of an unlabeled 4-set.
 * https://oeis.org/A005746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   G.f.: x*N(x)/D(x), with N(x) = 1 +3x +9x^2 +26x^3 +35x^4 +92x^5
 *            +127x^6 +201x^7 +242x^8 +253x^9 +248x^10 +205x^11 +123x^12
 *            +86x^13 +31x^14 +24x^15 +19x^16 +5x^17 +3x^18 -2x^19 -4x^20
 *            +2x^21 -4x^22 +3x^23 -x^25 +2x^26 -x^27 and D(x) =
 *            (1-x)^16*(1+x)^6*(1+x^2)^3*(1+x+x^2)^4 =
 *            (1-x)^6*(1-x^2)^3*(1-x...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 27, u64 n <= 144, u128 n <= 1000,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static const int A005746_num[28] = {
    1, 3, 9, 26, 35, 92, 127, 201, 242, 253, 248, 205, 123, 86,
    31, 24, 19, 5, 3, -2, -4, 2, -4, 3, 0, -1, 2, -1
};

seq_t a(idx_t n) {
    idx_t m = n - 1;
    seq_t *c = alloc_zero(m + 1);
    static const int ks[4] = {1, 2, 3, 4};
    static const int ms[4] = {6, 3, 4, 3};
    c[0] = 1;
    for (int t = 0; t < 4; t++) {
        idx_t k = (idx_t)ks[t];
        for (int r = 0; r < ms[t]; r++) {
            for (idx_t i = k; i <= m; i++) {
                c[i] += c[i - k];
            }
        }
    }
    seq_t pos = 0, neg = 0;
    for (idx_t i = 0; i < 28 && i <= m; i++) {
        int v = A005746_num[i];
        if (v > 0) {
            pos += c[m - i] * (seq_t)v;
        } else if (v < 0) {
            neg += c[m - i] * (seq_t)(-v);
        }
    }
    seq_t result = pos - neg;
    release(c, m + 1);
    return result;
}
