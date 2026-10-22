/*
 * A006256  a(n) = Sum_{k=0..n} binomial(3*k,k)*binomial(3*n-3*k,n-k).
 * https://oeis.org/A006256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=0..n} 3^(n-k)*binomial(2*n+k,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   a006256_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006256_gcd.h"

seq_t a(idx_t n) {
    seq_t r = 0;
    seq_t c = 1; /* binomial(2n+k, k) */
    idx_t k;
    for (k = 0; k <= n; k++) {
        if (k > 0) {
            /* c = c * (2n+k) / k, done without overflowing past the result */
            uint64_t rem = (uint64_t)(c % (seq_t)k);
            uint64_t g = a006256_gcd(k, rem);
            uint64_t kd = k / g;
            uint64_t m = (2 * n + k) / kd;
            c = (c / (seq_t)g) * (seq_t)m;
        }
        r = r * 3 + c;
    }
    return r;
}
