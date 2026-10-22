/*
 * A007054  Super ballot numbers: 6(2n)!/(n!(n+2)!).
 * https://oeis.org/A007054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(2n-1)*a(n-1)/(n+2), a(0) = 3 [i.e. a(n) =
 *            6(2n)!/(n!(n+2)!)]
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Helpers:   a007054_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a007054_gcd_u64.h"

seq_t a(idx_t n) {
    seq_t v = (seq_t)3;
    idx_t i;
    for (i = 1; i <= n; i++) {
        uint64_t k = 2 * (2 * (uint64_t)i - 1);
        uint64_t m = (uint64_t)i + 2;
        uint64_t g = a007054_gcd_u64(k, m);
        k /= g;
        m /= g;
        /* m is now coprime to k, so m divides v exactly */
        v = v / (seq_t)m;
        v = v * (seq_t)k;
    }
    return v;
}
