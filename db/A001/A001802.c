/*
 * A001802  Coefficients of Legendre polynomials.
 * https://oeis.org/A001802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*2^(n-4)*binomial(n+5/2, n)*2^A011371(n+6) =
 *            binomial(2n+6, n+3)*(n+1)*(n+2)*(n+3)/3 * 2^(A011371(n+6) - n -
 *            7), computed as odd part times a power of 2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 57,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 927 (2026-11-01)
 * Helpers:   gcd_u64, popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"
#include "../utils/popcount.h"

/* multiply O by x/y exactly (result known to be an integer), y odd */
static seq_t a1802_muldiv(seq_t O, uint64_t x, uint64_t y) {
    uint64_t r = (uint64_t)(O % (seq_t)y);
    uint64_t g = gcd_u64(y, r);
    O /= (seq_t)g;
    y /= g;
    return O * (seq_t)(x / y);
}

seq_t a(idx_t n) {
    uint64_t k = (uint64_t)n + 3;
    long long s = 0;
    seq_t O = 1;
    uint64_t i;
    uint64_t f[3];
    int j;

    /* odd part of binomial(2n+6, n+3); its 2-adic valuation is popcount(n+3) */
    for (i = 1; i <= k; i++) {
        uint64_t x = k + i, y = i;
        while ((x & 1) == 0) x >>= 1;
        while ((y & 1) == 0) y >>= 1;
        O = a1802_muldiv(O, x, y);
    }
    s += popcount(k);

    /* times (n+1)(n+2)(n+3)/3 */
    f[0] = (uint64_t)n + 1; f[1] = (uint64_t)n + 2; f[2] = (uint64_t)n + 3;
    for (j = 0; j < 3; j++) {
        if (f[j] % 3 == 0) { f[j] /= 3; break; }
    }
    for (j = 0; j < 3; j++) {
        uint64_t x = f[j];
        while ((x & 1) == 0) { x >>= 1; s++; }
        O *= (seq_t)x;
    }

    /* times 2^(A011371(n+6) - n - 7) */
    s += (long long)((uint64_t)n + 6) - popcount((uint64_t)n + 6);
    s -= (long long)n + 7;

    while (s > 0) {
        int step = s > 30 ? 30 : (int)s;
        O <<= step;
        s -= step;
    }
    while (s < 0) {
        O >>= 1;
        s++;
    }
    return O;
}
