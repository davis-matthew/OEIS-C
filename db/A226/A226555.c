/*
 * A226555  Numerators of row sums of the triangular enumeration of rational
 *          numbers A226314(n,k) / A054531(n,k), 1 <= k <= n.
 * https://oeis.org/A226555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{k=1..n} A226314(n,k)/A054531(n,k) * = S
 *            / gcd(S, n), where S = Sum_{k=1..n} (n*gcd(n,k) - (n-k)), *
 *            since A226314(n,k)/A054531(n,k) = (n*gcd(n,k) - (n-k)) / n
 * Range:     u8 n <= 11, u16 n <= 149, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 891 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static uint64_t A226555_gcd_u64(uint64_t x, uint64_t y) {
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t S = 0;
    for (idx_t k = 1; k <= n; k++) {
        uint64_t g = A226555_gcd_u64((uint64_t)n, (uint64_t)k);
        S += (seq_t)n * (seq_t)g - (seq_t)(n - k);
    }
    seq_t d = gcd(S, (seq_t)n);
    return S / d;
}
