/*
 * A000208  Number of even sequences with period 2n.
 * https://oeis.org/A000208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (A000013(2*n) + A000013(n))/2 if n is even,
 *            A000013(2*n)/2 if n is odd, where A000013(m) = Sum_{d|m}
 *            phi(2d)*2^(m/d) / (2m), A000013(0) = 1; a(0) = 1. Equivalently
 *            a(n) = T/(8n) with T = Sum_{d|2n} phi(2d)*2^(2n/d) + [n
 *            even]*2*Sum_{d|n} phi(2d)*2^(n/d)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Helpers:   a208_acc, a208_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a208_acc.h"
#include "../utils/a208_phi.h"

seq_t a(idx_t n)
{
    seq_t Q = 0;
    unsigned long long R = 0, M, m, d;
    if (n == 0) return (seq_t)1;
    M = 8ULL * (unsigned long long)n;
    m = 2ULL * (unsigned long long)n;
    for (d = 1; d <= m; d++)
        if (m % d == 0)
            a208_acc(&Q, &R, a208_phi(2 * d), m / d, M);
    if (n % 2 == 0) {
        m = (unsigned long long)n;
        for (d = 1; d <= m; d++)
            if (m % d == 0)
                a208_acc(&Q, &R, 2 * a208_phi(2 * d), m / d, M);
    }
    return Q + (seq_t)(R / M);
}
