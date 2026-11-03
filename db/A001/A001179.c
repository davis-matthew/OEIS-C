/*
 * A001179  Leonardo logarithm of n.
 * https://oeis.org/A001179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   iterate the Pisano period pi(m) = A001175(m), starting at m = n,
 *            until a fixed point is reached; that point is A235249(n) =
 *            24*5^(a(n)-1) for n > 1, so a(n) = 1 + (number of factors 5 in
 *            it/24); a(1) = 0. pi(m) = lcm over p^e || m of p^(e-1)*pi(p),
 *            with pi(p) found by iterating the Fibonacci numb...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9592 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

static uint64_t a1179_pisano_prime(uint64_t p) {
    uint64_t f0 = 0, f1 = 1, k = 0;
    do {
        uint64_t t = (f0 + f1) % p;
        f0 = f1;
        f1 = t;
        k++;
    } while (!(f0 == 0 && f1 == 1));
    return k;
}

static uint64_t a1179_pisano(uint64_t m) {
    uint64_t res = 1, p;
    if (m == 1) return 1;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t pe = 1, per;
            m /= p;
            while (m % p == 0) { m /= p; pe *= p; }
            per = a1179_pisano_prime(p) * pe;
            res = res / gcd_u64(res, per) * per;
        }
    }
    if (m > 1) {
        uint64_t per = a1179_pisano_prime(m);
        res = res / gcd_u64(res, per) * per;
    }
    return res;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n, next, q;
    seq_t k = 0;
    for (;;) {
        next = a1179_pisano(m);
        if (next == m) break;
        m = next;
    }
    if (m == 1) return (seq_t)0;
    q = m / 24;
    k = 1;
    while (q % 5 == 0) { q /= 5; k++; }
    return k;
}
