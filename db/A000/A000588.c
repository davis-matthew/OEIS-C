/*
 * A000588  a(n) = 7*binomial(2n,n-3)/(n+4).
 * https://oeis.org/A000588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*binomial(2n,n-3)/(n+4); computed via a(3) = 1, a(n+1) =
 *            a(n) * 2*(2n+1)*(n+1) / ((n-2)*(n+5)) for n >= 3; a(0..2) = 0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   a588_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a588_gcd.h"

seq_t a(idx_t n) {
    seq_t v;
    idx_t k;
    if (n < 3) return (seq_t)0;
    v = (seq_t)1;
    for (k = 3; k < n; k++) {
        uint64_t num = (uint64_t)2 * (2 * (uint64_t)k + 1) * ((uint64_t)k + 1);
        uint64_t den = ((uint64_t)k - 2) * ((uint64_t)k + 5);
        uint64_t g = a588_gcd(num, den);
        num /= g;
        den /= g;
        /* den is coprime to num, so it divides v exactly */
        v /= den;
        v *= num;
    }
    return v;
}
