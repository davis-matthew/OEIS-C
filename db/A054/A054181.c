/*
 * A054181  Inverse Moebius transform of A000011 (starting at term 0).
 * https://oeis.org/A054181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d | n+1} A000011(d-1), i.e. the inverse Moebius
 *            transform of * b(k) = A000011(k-1) for k >= 1, where *
 *            A000011(m) = (Sum_{d|m} phi(2d)*2^(m/d) + 2m*2^floor(m/2)) /
 *            (4m) for m >= 1, A000011(0) = 1
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-07)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

/* Adds m * r0 * 2^k (r0 < M) to the running value Q*M + R, without forming the full product. */
static void A000011_add_term(seq_t *Q, unsigned long long *R,
                             unsigned long long r0, unsigned long long k,
                             unsigned long long m, unsigned long long M)
{
    seq_t q = 0;
    unsigned long long r = r0, i;
    for (i = 0; i < k; i++) {
        q = q * 2;
        r = r * 2;
        if (r >= M) { r -= M; q += 1; }
    }
    q = q * m + (seq_t)((r * m) / M);
    r = (r * m) % M;
    *Q += q;
    *R += r;
    while (*R >= M) { *R -= M; *Q += 1; }
}

static seq_t A000011_a(idx_t n)
{
    unsigned long long nn, M, d;
    seq_t Q = 0;
    unsigned long long R = 0;
    if (n == 0) return (seq_t)1;
    nn = (unsigned long long)n;
    M = 4 * nn;
    for (d = 1; d * d <= nn; d++) {
        if (nn % d == 0) {
            unsigned long long e = nn / d;
            A000011_add_term(&Q, &R, 1, e, totient_u64(2 * d), M);
            if (e != d)
                A000011_add_term(&Q, &R, 1, d, totient_u64(2 * e), M);
        }
    }
    A000011_add_term(&Q, &R, 2 * nn, nn / 2, 1, M);
    return Q;
}

seq_t a(idx_t n)
{
    unsigned long long m, d;
    seq_t s = 0;
    m = (unsigned long long)n + 1;
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            unsigned long long e = m / d;
            s += A000011_a((idx_t)(d - 1));
            if (e != d)
                s += A000011_a((idx_t)(e - 1));
        }
    }
    return s;
}
