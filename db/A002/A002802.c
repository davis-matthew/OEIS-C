/*
 * A002802  a(n) = (2*n+3)!/(6*n!*(n+1)!).
 * https://oeis.org/A002802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * 2*(2n+3)/n, a(0) = 1 (i.e. a(n) =
 *            (2*n+3)!/(6*n!*(n+1)!))
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   a2802_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a2802_gcd.h"

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        /* v * (2*(2k+3)) / k, dividing first to avoid intermediate overflow */
        uint64_t r = (uint64_t)(v % (seq_t)k);
        uint64_t g = a2802_gcd(r, (uint64_t)k);
        uint64_t q = (uint64_t)k / g;
        uint64_t num = 2 * (2 * (uint64_t)k + 3);
        uint64_t t = num / q;  /* exact: q divides num since gcd(v/g, q) = 1 */
        v = (v / (seq_t)g) * (seq_t)t;
    }
    return v;
}
