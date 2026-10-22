/*
 * A002736  Apéry numbers: a(n) = n^2*C(2n,n).
 * https://oeis.org/A002736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n^2*C(2n,n), with C(2k,k) = C(2k-2,k-1)*2*(2k-1)/k,
 *            C(0,0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   a002736_gcd_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002736_gcd_u.h"

seq_t a(idx_t n) {
    seq_t c = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        uint64_t r = (uint64_t)(c % (seq_t)k);
        uint64_t g = a002736_gcd_u((uint64_t)k, r);
        uint64_t kg = (uint64_t)k / g;
        uint64_t m = 2 * (2 * (uint64_t)k - 1);
        c = (c / (seq_t)g) * (seq_t)(m / kg);
    }
    return (seq_t)n * (seq_t)n * c;
}
