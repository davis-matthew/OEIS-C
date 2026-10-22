/*
 * A004322  Binomial coefficient C(3n,n-4).
 * https://oeis.org/A004322
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(3n, n-4), computed multiplicatively as prod_{i=1..n-4}
 *            (2n+4+i)/i
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 49,
 *            big any n
 * Helpers:   a004322_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004322_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 4;
    idx_t base = 2 * n + 4;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        uint64_t rm = (uint64_t)(r % (seq_t)i);
        uint64_t g = a004322_gcd(rm, (uint64_t)i);
        uint64_t den = (uint64_t)i / g;
        seq_t num = (seq_t)(base + i);
        r /= (seq_t)g;
        num /= (seq_t)den;
        r *= num;
    }
    return r;
}
