/*
 * A002005  Number of rooted planar cubic maps with 2n vertices.
 * https://oeis.org/A002005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n+1)*(3*n)!!/((n+2)!*n!!), computed via a(n) =
 *            a(n-2) * 48*(3n-2)*(3n-4) / ((n+1)*(n+2)), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   a002005_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002005_gcd_u64.h"

seq_t a(idx_t n) {
    seq_t r;
    idx_t k;
    if (n % 2 == 0) {
        r = 1;
        k = 2;
    } else {
        r = 4;
        k = 3;
    }
    for (; k <= n; k += 2) {
        uint64_t num = (uint64_t)48 * (3 * k - 2) * (3 * k - 4);
        uint64_t den = (uint64_t)(k + 1) * (k + 2);
        uint64_t g = a002005_gcd_u64(num, den);
        num /= g;
        den /= g;
        /* den is coprime to num, so it divides r exactly */
        r = r / (seq_t)den;
        r = r * (seq_t)num;
    }
    return r;
}
