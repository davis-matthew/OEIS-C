/*
 * A004370  Binomial coefficient C(7n,n-2).
 * https://oeis.org/A004370
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(7n, n-2), computed multiplicatively as r_{i+1} = r_i *
 *            (7n-i) / (i+1), with each fraction first reduced by its gcd
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   a004370_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004370_gcd.h"

seq_t a(idx_t n) {
    idx_t m = 7 * n;
    idx_t k = n - 2;
    seq_t r = 1;
    idx_t i;
    for (i = 0; i < k; i++) {
        uint64_t num = (uint64_t)(m - i);
        uint64_t den = (uint64_t)(i + 1);
        uint64_t g = a004370_gcd(num, den);
        num /= g;
        den /= g;
        /* den is coprime to num and divides r*num, so den divides r */
        r = (r / (seq_t)den) * (seq_t)num;
    }
    return r;
}
