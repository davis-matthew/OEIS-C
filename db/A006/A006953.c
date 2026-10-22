/*
 * A006953  a(n) = denominator of Bernoulli(2n)/(2n).
 * https://oeis.org/A006953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   a(n) = Product_{p prime, (p-1) | 2n} p^(1 + v_p(2n)) (von
 *            Staudt-Clausen with the Adams/Kummer congruences: the
 *            denominator of Bernoulli(2n)/(2n) is the product of
 *            p^(1+v_p(2n)) over primes p with p-1 | 2n)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 35, u64 n <= 209,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t factor_for(uint64_t d, uint64_t m) {
    uint64_t p = d + 1;
    if (!is_prime(p)) return (seq_t)1;
    seq_t f = (seq_t)p;
    uint64_t t = m;
    while (t % p == 0) {
        f *= (seq_t)p;
        t /= p;
    }
    return f;
}

seq_t a(idx_t n) {
    uint64_t m = 2 * (uint64_t)n;
    seq_t r = 1;
    uint64_t d;
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            uint64_t e = m / d;
            r *= factor_for(d, m);
            if (e != d) r *= factor_for(e, m);
        }
    }
    return r;
}
