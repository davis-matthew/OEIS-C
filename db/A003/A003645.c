/*
 * A003645  a(n) = 2^n * C(n+1), where C(n) = A000108(n) Catalan numbers.
 * https://oeis.org/A003645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * 4*(2n+1)/(n+2), a(0) = 1 (a(n) = 2^n * C(n+1))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   a003645_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a003645_gcd.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    for (idx_t k = 1; k <= n; k++) {
        uint64_t den = (uint64_t)k + 2;
        uint64_t num = 4 * (2 * (uint64_t)k + 1);
        uint64_t rem = (uint64_t)(r % (seq_t)den);
        uint64_t g = a003645_gcd(den, rem);
        uint64_t d = den / g;
        r = (r / (seq_t)g) * (seq_t)(num / d);
    }
    return r;
}
