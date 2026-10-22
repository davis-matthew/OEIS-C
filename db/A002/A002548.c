/*
 * A002548  Denominators of coefficients for numerical differentiation.
 * https://oeis.org/A002548
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of 2*H(n-1)/n (equivalently of 1 -
 *            2*H(n-1)/n), H = harmonic numbers. Writing H(n-1) = S/L with L =
 *            lcm(1..n-1), S = Sum_{k<n} L/k, a(n) = n*L / gcd(2*S, n*L); the
 *            p-adic valuation of 2*S is computed modulo p^(v_p(n*L)) for each
 *            prime p <= n
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 25, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)1;
    char *comp = (char *)calloc((size_t)n + 1, 1);
    seq_t r = 1;
    for (uint64_t p = 2; p <= n; p++) {
        if (comp[p]) continue;
        for (uint64_t q = p * p; q <= n; q += p) comp[q] = 1;

        /* eL = v_p(lcm(1..n-1)), en = v_p(n) */
        uint64_t eL = 0, pw = 1;
        while (pw <= (n - 1) / p) { pw *= p; eL++; }
        uint64_t en = 0, t = n;
        while (t % p == 0) { t /= p; en++; }
        uint64_t E = eL + en;
        if (E == 0) continue;
        uint64_t M = 1;
        for (uint64_t i = 0; i < E; i++) M *= p;

        /* T = Sum_{k<n} p^(eL - v_p(k)) / k' (mod M), k' = k / p^v_p(k); kept as A/B with B a unit */
        uint64_t A = 0, B = 1;
        for (uint64_t k = 1; k < n; k++) {
            uint64_t kk = k, v = 0;
            while (kk % p == 0) { kk /= p; v++; }
            uint64_t pj = 1;
            for (uint64_t i = v; i < eL; i++) pj *= p;
            uint64_t km = kk % M;
            A = (A * km + (pj % M) * B) % M;
            B = (B * km) % M;
        }
        /* valuation of 2*T, capped at E */
        uint64_t val = (p == 2) ? 1 : 0;
        uint64_t x = A;
        while (val < E) {
            if (x == 0) { val = E; break; }
            if (x % p != 0) break;
            x /= p;
            val++;
        }
        if (val > E) val = E;
        for (uint64_t i = val; i < E; i++) r *= (seq_t)p;
    }
    free(comp);
    return r;
}
