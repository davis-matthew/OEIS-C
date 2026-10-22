/*
 * A002595  Denominators of Taylor series expansion of arcsin(x). Also arises
 *          from arccos(x), arccsc(x), arcsec(x), arcsinh(x).
 * https://oeis.org/A002595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   a(n) = denominator((2*n)!/(2^(2*n)*(n)!^2*(2*n+1))) = 2^(2n -
 *            popcount(n)) * (2n+1)/gcd(binomial(2n,n), 2n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   a2595_vfact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a2595_vfact.h"

seq_t a(idx_t n) {
    uint64_t m = 2 * (uint64_t)n + 1;
    uint64_t rest = m, odd = m, p;
    for (p = 3; p * p <= rest; p += 2) {
        if (rest % p == 0) {
            uint64_t e = 0, pe = 1, vb, k;
            while (rest % p == 0) { rest /= p; e++; }
            vb = a2595_vfact(2 * (uint64_t)n, p) - 2 * a2595_vfact((uint64_t)n, p);
            k = vb < e ? vb : e;
            while (k--) pe *= p;
            odd /= pe;
        }
    }
    if (rest > 1) {
        uint64_t vb = a2595_vfact(2 * (uint64_t)n, rest) - 2 * a2595_vfact((uint64_t)n, rest);
        if (vb >= 1) odd /= rest;
    }
    {
        uint64_t pc = 0, t = n;
        uint64_t e;
        seq_t base = 2, r = 1;
        while (t) { pc += t & 1; t >>= 1; }
        e = 2 * (uint64_t)n - pc;
        while (e) {
            if (e & 1) r *= base;
            e >>= 1;
            if (e) base *= base;
        }
        return r * (seq_t)odd;
    }
}
