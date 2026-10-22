/*
 * A003519  a(n) = 10*binomial(2*n + 1, n - 4)/(n + 6).
 * https://oeis.org/A003519
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*binomial(2*n+1, n-4)/(n+6); computed via a(n+1) =
 *            2*(n+1)*(2*n+3)/((n+7)*(n-3)) * a(n), a(4) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   a3519_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3519_gcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t i;
    if (n < 4) return 0;
    for (i = 4; i < n; i++) {
        uint64_t num = 2 * ((uint64_t)i + 1) * (2 * (uint64_t)i + 3);
        uint64_t den = ((uint64_t)i + 7) * ((uint64_t)i - 3);
        uint64_t g = a3519_gcd(num, den);
        num /= g;
        den /= g;
        /* den is coprime to num, so it divides result exactly */
        result = result / (seq_t)den * (seq_t)num;
    }
    return result;
}
