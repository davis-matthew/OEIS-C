/*
 * A007967  Second factor in happy factorization of n.
 * https://oeis.org/A007967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = sqrt(n) if n is a square; otherwise, with (p,q) the
 *            fundamental solution of p^2 - n*q^2 = 1 (found from the
 *            continued fraction of sqrt(n), working mod 2n): a(n) = gcd(n,
 *            p+1) if q is odd, a(n) = gcd(n, (p+1)/2) if q is even
 * Range:     u8 n <= 256, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   a7967_gcd, a7967_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a7967_gcd.h"
#include "../utils/a7967_isqrt.h"

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t r = a7967_isqrt(N);
    if (r * r == N) return (seq_t)r;

    uint64_t M = 2 * N;
    uint64_t a0 = r;
    uint64_t m = 0, d = 1, ai = a0;
    uint64_t hm1 = 1, hm2 = 0;   /* h_{-1}, h_{-2} mod M */
    uint64_t km1 = 0, km2 = 1;   /* k_{-1}, k_{-2} mod M */
    uint64_t h = 0, k = 0;
    uint64_t i = 0;
    for (;;) {
        h = ((ai % M) * hm1 + hm2) % M;
        k = ((ai % M) * km1 + km2) % M;
        hm2 = hm1; hm1 = h;
        km2 = km1; km1 = k;
        /* next partial quotient */
        m = d * ai - m;
        d = (N - m * m) / d;
        ai = (a0 + m) / d;
        if (ai == 2 * a0 && ((i + 1) % 2 == 0)) break;
        i++;
    }

    uint64_t y;
    if (k % 2 == 1) {
        uint64_t t = (h + 1) % M % N;
        y = a7967_gcd(N, t);
    } else {
        uint64_t t = ((h + 1) % M) / 2;
        y = a7967_gcd(N, t % N);
    }
    return (seq_t)y;
}
