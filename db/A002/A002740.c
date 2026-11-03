/*
 * A002740  Number of tree-rooted bridgeless planar maps with two vertices and
 *          n faces.
 * https://oeis.org/A002740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*binomial(2n-2, n-2)/2 + 0^n
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 198 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    idx_t N = 2 * n - 2;
    idx_t k = n - 2;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        uint64_t num = (uint64_t)(N - k + i);
        uint64_t den = (uint64_t)i;
        uint64_t rm = (uint64_t)(long long)(r % (seq_t)den);
        uint64_t g = gcd_u64(den, rm);
        r /= (seq_t)g;
        den /= g;
        r *= (seq_t)(num / den);
    }
    if (k % 2 == 0) {
        return r * (seq_t)(k / 2);
    }
    return (r / 2) * (seq_t)k;
}
