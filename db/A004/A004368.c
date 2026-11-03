/*
 * A004368  Binomial coefficient C(7n,n).
 * https://oeis.org/A004368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(7n,n) = Product_{i=1..n} (6n+i)/i
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 99 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        /* r = C(6n+i-1, i-1); next r = r*(6n+i)/i, done without overflow */
        uint64_t rm = (uint64_t)(r % (seq_t)i);
        uint64_t g = gcd_u64((uint64_t)i, rm);
        uint64_t ig = (uint64_t)i / g;
        uint64_t num = (uint64_t)(6 * n + i) / ig;
        r /= (seq_t)g;
        r *= (seq_t)num;
    }
    return r;
}
