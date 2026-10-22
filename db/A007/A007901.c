/*
 * A007901  Number of minimal unavoidable n-celled pebbling configurations.
 * https://oeis.org/A007901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f.:
 *            x^3*((1-3*x+x^2)*sqrt(1-4*x)-1+5*x-x^2-6*x^3)/(1-7*x+14*x^2-9*x^3).
 *            With S(x) = Sum_{k>=1} C(k-1)*x^k (Catalan numbers C),
 *            sqrt(1-4x) = 1 - 2*S(x), so a(n) = b(n-3), where b(k) = 7*b(k-1)
 *            - 14*b(k-2) + 9*b(k-3) + N(k), N(k) = 2*[k=1] - 6*[k=3] -
 *            2*(S(k) - 3*S(k-1) + S(k-2)), b(k<0) = 0, S(0...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 66,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/grow_zero.h"

static seq_t *bv = NULL;   /* bv[k] = b(k) */
static seq_t *sv = NULL;   /* sv[k] = S(k) = Catalan(k-1) */
static idx_t blen = 0;

static uint64_t small_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n) {
    idx_t m, newlen, k;
    if (n < 4) return 0;
    m = n - 3;
    if (m + 1 > blen) {
        newlen = blen * 2;
        if (newlen < m + 1) newlen = m + 1;
        if (newlen < 16) newlen = 16;
        bv = grow_zero(bv, blen, newlen);
        sv = grow_zero(sv, blen, newlen);
        for (k = blen; k < newlen; k++) {
            seq_t pos, neg;
            /* Catalan part */
            if (k == 0) sv[k] = 0;
            else if (k == 1) sv[k] = 1;
            else {
                uint64_t j = (uint64_t)(k - 1);
                uint64_t mul = 4 * j - 2, d = j + 1, g = small_gcd(mul, d);
                mul /= g; d /= g;
                sv[k] = sv[k - 1] / d * mul;
            }
            /* b(k) = positive parts - negative parts (result is nonnegative) */
            pos = 0; neg = 0;
            if (k >= 1) { pos += 7 * bv[k - 1]; pos += 6 * sv[k - 1]; }
            if (k >= 3) pos += 9 * bv[k - 3];
            if (k == 1) pos += 2;
            neg += 2 * sv[k];
            if (k >= 2) { neg += 14 * bv[k - 2]; neg += 2 * sv[k - 2]; }
            if (k == 3) neg += 6;
            bv[k] = pos - neg;
        }
        blen = newlen;
    }
    return bv[m];
}
