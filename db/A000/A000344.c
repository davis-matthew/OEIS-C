/*
 * A000344  a(n) = 5*binomial(2n, n-2)/(n+3).
 * https://oeis.org/A000344
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n+3)*(n-2)*a(n) = 2*n*(2*n-1)*a(n-1), a(2) = 1 (i.e. a(n) =
 *            5*binomial(2n, n-2)/(n+3))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   a000344_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000344_gcd.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t k;
    for (k = 3; k <= n; k++) {
        uint64_t num = (uint64_t)(2 * k) * (uint64_t)(2 * k - 1);
        uint64_t den = (uint64_t)(k + 3) * (uint64_t)(k - 2);
        uint64_t g = a000344_gcd(num, den);
        num /= g;
        den /= g;
        /* den is coprime to num and den*a(k) = num*a(k-1), so den divides a(k-1) */
        r = r / (seq_t)den;
        r = r * (seq_t)num;
    }
    return r;
}
