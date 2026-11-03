/*
 * A000206  Even sequences with period 2n.
 * https://oeis.org/A000206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1; with S(m) = Sum_{d|m} phi(2d)*2^(m/d) (so A000013(m) =
 *            S(m)/(2m)), a(n) = (S(2n) + 2*S(n) + 6n*2^n)/(16n) if n is even,
 *            a(n) = (S(2n) + 4n*2^n)/(16n) if n is odd. (Equivalent to a(n) =
 *            (A000011(2n)+A000011(n)+4^(n/2-1)-2^(n/2-1))/2 for even n, a(n)
 *            = A000011(2n)/2 for odd n.)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 971 (2026-11-01)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

/* 2^k = q*D + r, with q computed in seq_t (wrapping is fine for fixed width) */
static void a206_pdiv(idx_t k, uint64_t D, seq_t *q, uint64_t *r)
{
    seq_t Q = 0;
    uint64_t R = 1 % D;
    while (k > 0) {
        unsigned s = k > 15 ? 15u : (unsigned)k;
        uint64_t t = R << s;
        Q = (seq_t)(Q << s);
        Q += (idx_t)(t / D);
        R = t % D;
        k -= s;
    }
    *q = Q;
    *r = R;
}

static void a206_add(seq_t *Q, uint64_t *R, uint64_t D, uint64_t c, idx_t k)
{
    seq_t q;
    uint64_t r;
    a206_pdiv(k, D, &q, &r);
    *Q += q * (idx_t)c;
    *R += c * r;
    *Q += (idx_t)(*R / D);
    *R %= D;
}

seq_t a(idx_t n)
{
    seq_t Q = 0;
    uint64_t R = 0, D, d, m;
    if (n == 0) return (seq_t)1;
    D = 16 * (uint64_t)n;
    m = 2 * (uint64_t)n;
    for (d = 1; d <= m; d++)
        if (m % d == 0)
            a206_add(&Q, &R, D, totient_u64(2 * d), m / d);
    if (n % 2 == 0) {
        for (d = 1; d <= n; d++)
            if (n % d == 0)
                a206_add(&Q, &R, D, 2 * totient_u64(2 * d), n / d);
        a206_add(&Q, &R, D, 6 * (uint64_t)n, n);
    } else {
        a206_add(&Q, &R, D, 4 * (uint64_t)n, n);
    }
    return Q;
}
