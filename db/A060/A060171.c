/*
 * A060171  Number of orbits of length n under a map whose periodic points
 *          seem to be counted by A006953.
 * https://oeis.org/A060171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/n)*Sum_{d|n} mu(d)*A006953(n/d), A006953(n) =
 *            Product_{p prime, (p-1)|2n} p^(1+v_p(2n))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static seq_t A006953_factor_for(uint64_t d, uint64_t m) {
    uint64_t p = d + 1;
    if (!is_prime_u64(p)) return (seq_t)1;
    seq_t f = (seq_t)p;
    uint64_t t = m;
    while (t % p == 0) {
        f *= (seq_t)p;
        t /= p;
    }
    return f;
}

static seq_t A006953_a(idx_t n) {
    uint64_t m = 2 * (uint64_t)n;
    seq_t r = 1;
    uint64_t d;
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            uint64_t e = m / d;
            r *= A006953_factor_for(d, m);
            if (e != d) r *= A006953_factor_for(e, m);
        }
    }
    return r;
}

seq_t a(idx_t n) {
    uint64_t pr[20];
    int k = 0;
    uint64_t t = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            pr[k++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) pr[k++] = t;

    seq_t pos = 0, neg = 0;
    uint32_t mask;
    for (mask = 0; mask < (1u << k); mask++) {
        uint64_t d = 1;
        int bits = 0;
        int i;
        for (i = 0; i < k; i++) {
            if (mask & (1u << i)) {
                d *= pr[i];
                bits++;
            }
        }
        seq_t v = A006953_a((idx_t)((uint64_t)n / d));
        if (bits & 1) neg += v;
        else pos += v;
    }
    return (pos - neg) / (seq_t)n;
}
