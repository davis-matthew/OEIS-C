/*
 * A006863  Denominator of B_{2n}/(-4n), where B_m are the Bernoulli numbers.
 * https://oeis.org/A006863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, nice
 * Formula:   a(0) = 1; for n > 0, a(n) = 2^(a+2) * Product p^(b+1), where 2^a
 *            exactly divides 2n and p runs over the odd primes with (p-1) |
 *            2n, p^b exactly dividing 2n
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 29, u64 n <= 209,
 *            u128 n <= 1259, big any n
 * Validated: standard, pass, 48 terms through n = 8906 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static void a6863_mulp(seq_t *result, idx_t m, idx_t p) {
    idx_t b = 0;
    idx_t mc = m;
    while (mc % p == 0) {
        b++;
        mc /= p;
    }
    for (idx_t i = 0; i <= b; i++) {
        *result *= (seq_t)p;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;

    idx_t m = 2 * n;
    seq_t result = 1;

    idx_t a2 = 0;
    idx_t m2 = m;
    while (m2 % 2 == 0) {
        a2++;
        m2 /= 2;
    }
    for (idx_t i = 0; i < a2 + 2; i++) {
        result *= 2;
    }

    for (idx_t d = 1; d <= m / d; d++) {
        if (m % d == 0) {
            idx_t p = d + 1;
            if (p > 2 && is_prime_u64(p)) a6863_mulp(&result, m, p);
            idx_t d2 = m / d;
            if (d2 != d) {
                p = d2 + 1;
                if (p > 2 && is_prime_u64(p)) a6863_mulp(&result, m, p);
            }
        }
    }

    return result;
}
