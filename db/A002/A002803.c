/*
 * A002803  a(n) = (2n+4)!/(4!*n!*(n+1)!).
 * https://oeis.org/A002803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * 2*(n+2)*(2*n+3) / (n*(n+1)), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 57,
 *            big any n
 * Helpers:   a2803_small_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a2803_small_gcd.h"

seq_t a(idx_t n) {
    seq_t result = (seq_t)1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        uint64_t num = (uint64_t)2 * (uint64_t)(k + 2) * (uint64_t)(2 * k + 3);
        uint64_t den = (uint64_t)k * (uint64_t)(k + 1);
        uint64_t g = a2803_small_gcd(num, den);
        num /= g;
        den /= g;
        /* den is coprime to num and divides result*num, so it divides result */
        result = result / (seq_t)den;
        result = result * (seq_t)num;
    }
    return result;
}
