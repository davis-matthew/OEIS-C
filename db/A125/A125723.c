/*
 * A125723  Greatest common divisor of n^6 and 6^n.
 * https://oeis.org/A125723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n^6, 6^n) Implementation uses the fact that only the
 *            primes 2 and 3 appear in 6^n, so the gcd is 2^{min(6*v2(n), n)}
 *            * 3^{min(6*v3(n), n)}
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 47, u64 n <= 1727,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 64 terms through n = 8995 (2026-11-01)
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    /* Count powers of 2 and 3 in n */
    idx_t tmp = n;
    idx_t v2 = 0, v3 = 0;
    while (tmp % 2 == 0) {
        v2++;
        tmp /= 2;
    }
    while (tmp % 3 == 0) {
        v3++;
        tmp /= 3;
    }

    /* Compute exponents of 2 and 3 in the gcd, capped by n */
    idx_t exp2 = 6 * v2;
    if (exp2 > n) exp2 = n;
    idx_t exp3 = 6 * v3;
    if (exp3 > n) exp3 = n;

    /* Result = 2^exp2 * 3^exp3 */
    seq_t res2 = pow2(exp2);
    seq_t res3 = pow3(exp3);
    return seq_mul(res2, res3);
}
