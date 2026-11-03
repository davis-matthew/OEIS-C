/*
 * A005714  Coefficient of x^6 in expansion of (1+x+x^2)^n.
 * https://oeis.org/A005714
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,3) + 6*binomial(n,4) + 5*binomial(n,5) +
 *            binomial(n,6)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 118, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 857 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"

/* binomial(n, k) for small k, exact whenever the result fits in seq_t */
static seq_t small_binom(idx_t n, unsigned k) {
    uint64_t f[8];
    unsigned i, d;
    seq_t r = 1;
    if (n < k) return 0;
    for (i = 0; i < k; i++) f[i] = (uint64_t)(n - i);
    for (d = 2; d <= k; d++) {
        uint64_t rem = d;
        for (i = 0; i < k && rem > 1; i++) {
            uint64_t g = gcd_u64(f[i], rem);
            f[i] /= g;
            rem /= g;
        }
    }
    for (i = 0; i < k; i++) r *= (seq_t)f[i];
    return r;
}

seq_t a(idx_t n) {
    seq_t r = small_binom(n, 3);
    r += small_binom(n, 4) * 6;
    r += small_binom(n, 5) * 5;
    r += small_binom(n, 6);
    return r;
}
