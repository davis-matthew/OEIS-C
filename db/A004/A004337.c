/*
 * A004337  Binomial coefficient C(4n,n-7).
 * https://oeis.org/A004337
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n, n-7), computed multiplicatively as
 *            C(3n+7+i, i) = C(3n+7+i-1, i-1) * (3n+7+i) / i for i = 1..n-7,
 *            with gcd reduction so no intermediate exceeds the final value
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 15, u64 n <= 24, u128 n <= 44,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n) {
    idx_t k = n - 7;
    idx_t base = 3 * n + 7; /* m - k where m = 4n */
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        uint64_t num = (uint64_t)(base + i);
        uint64_t den = (uint64_t)i;
        uint64_t rm = (uint64_t)(r % (seq_t)den);
        uint64_t g = gcd_u64(rm, den);
        /* r*num/den is an integer; with g = gcd(r, den), den/g divides num */
        r /= (seq_t)g;
        den /= g;
        num /= den;
        r *= (seq_t)num;
    }
    return r;
}
