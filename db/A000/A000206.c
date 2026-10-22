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
 * Helpers:   a206_add, a206_pdiv, a206_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a206_add.h"
#include "../utils/a206_pdiv.h"
#include "../utils/a206_phi.h"

seq_t a(idx_t n)
{
    seq_t Q = 0;
    uint64_t R = 0, D, d, m;
    if (n == 0) return (seq_t)1;
    D = 16 * (uint64_t)n;
    m = 2 * (uint64_t)n;
    for (d = 1; d <= m; d++)
        if (m % d == 0)
            a206_add(&Q, &R, D, a206_phi(2 * d), m / d);
    if (n % 2 == 0) {
        for (d = 1; d <= n; d++)
            if (n % d == 0)
                a206_add(&Q, &R, D, 2 * a206_phi(2 * d), n / d);
        a206_add(&Q, &R, D, 6 * (uint64_t)n, n);
    } else {
        a206_add(&Q, &R, D, 4 * (uint64_t)n, n);
    }
    return Q;
}
