/*
 * A060404  G.f.: Sum_{k >= 1} (phi(k)/k)*log(1-f(x^k)), where f(x) = (1 -
 *          sqrt(1 - 4*x)) / (2*x) - 1 is the g.f. for the Catalan numbers
 *          (A000108) C_1, C_2, C_3, ...
 * https://oeis.org/A060404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} phi(n/d) * A000346(d-1) for n > 0, a(0)
 *            = 0, * where A000346(k) = 4*A000346(k-1) + C(k), A000346(0) = 1,
 *            C(k) = Catalan. * Every A000346 value is held as q*n + r, so the
 *            large d = n term never * overflows before the division by n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 197 (2026-11-07)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

static uint64_t A060404_phi(uint64_t m)
{
    uint64_t r = m, p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            r -= r / p;
        }
    }
    if (m > 1) r -= r / m;
    return r;
}

seq_t a(idx_t n)
{
    seq_t N, c, q, Q;
    uint64_t r, R, nn;
    idx_t k;
    if (n == 0) return (seq_t)0;
    nn = (uint64_t)n;
    N = (seq_t)n;
    c = (seq_t)1;          /* Catalan C(k) */
    /* A000346(0) = 1 = q*n + r */
    q = (seq_t)0;
    r = 1;
    if (r >= nn) { r -= nn; q += 1; }
    Q = (seq_t)0;
    R = 0;
    for (k = 0; k < n; k++) {
        if (k > 0) {
            unsigned long long m = 2ULL * (2ULL * (unsigned long long)k - 1ULL);
            unsigned long long d = (unsigned long long)k + 1ULL;
            unsigned long long rr = (unsigned long long)(c % (seq_t)d);
            unsigned long long g = gcd_u64(d, rr);
            seq_t cq;
            uint64_t cr, t;
            c /= (seq_t)g;
            d /= g;
            c *= (seq_t)(m / d);
            cq = c / N;
            cr = (uint64_t)(c % N);
            t = 4 * r + cr;
            q = q * 4 + cq + (seq_t)(t / nn);
            r = t % nn;
        }
        if (n % (k + 1) == 0) {
            uint64_t ph = A060404_phi((uint64_t)(n / (k + 1)));
            uint64_t u = ph * r;
            Q += (seq_t)ph * q;
            Q += (seq_t)(u / nn);
            R += u % nn;
            if (R >= nn) { R -= nn; Q += 1; }
        }
    }
    return Q;
}
