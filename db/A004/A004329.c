/*
 * A004329  Binomial coefficient C(3n,n-11).
 * https://oeis.org/A004329
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(3n, n-11), computed multiplicatively as C(2n+11+i, i) =
 *            C(2n+10+i, i-1) * (2n+11+i) / i for i = 1..n-11
 * Range:     u8 n <= 12, u16 n <= 14, u32 n <= 19, u64 n <= 30, u128 n <= 52,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n) {
    if (n < 11) return 0;
    idx_t k = n - 11;
    idx_t base = 2 * n + 11;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; i++) {
        uint64_t rm = (uint64_t)(r % (seq_t)i);
        uint64_t g = gcd_u64(rm == 0 ? (uint64_t)i : rm, (uint64_t)i);
        r /= (seq_t)g;
        uint64_t d = (uint64_t)i / g;
        seq_t t = ((seq_t)base + (seq_t)i) / (seq_t)d;
        r *= t;
    }
    return r;
}
