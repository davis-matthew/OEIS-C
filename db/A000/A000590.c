/*
 * A000590  a(n) = 13*binomial(2n,n-6)/(n+7).
 * https://oeis.org/A000590
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*binomial(2n,n-6)/(n+7); computed by -(n+7)*(n-6)*a(n)
 *            + 2*n*(2*n-1)*a(n-1) = 0, a(6) = 1
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   a590_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a590_gcd.h"

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t k;
    if (n < 6) return 0;
    for (k = 7; k <= n; k++) {
        uint64_t num = (uint64_t)(2 * k) * (uint64_t)(2 * k - 1);
        uint64_t den = (uint64_t)(k + 7) * (uint64_t)(k - 6);
        uint64_t g = a590_gcd(num, den);
        seq_t d;
        num /= g;
        den /= g;
        d = (seq_t)den;
        if (d == 0) d = 1; /* only possible after overflow in narrow types */
        v = (v / d) * (seq_t)num;
    }
    return v;
}
