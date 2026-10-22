/*
 * A002458  a(n) = binomial(4*n+1, 2*n).
 * https://oeis.org/A002458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = binomial(4*n+1, 2*n), computed multiplicatively as
 *            C(2n+1+i, i) = C(2n+i, i-1) * (2n+1+i) / i for i = 1..2n, with
 *            gcd cancellation so intermediates never exceed the final value
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   a002458_gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002458_gcd64.h"

seq_t a(idx_t n) {
    idx_t k = 2 * n;
    idx_t base = 2 * n + 1;
    seq_t result = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        uint64_t num = (uint64_t)(base + i);
        uint64_t r = (uint64_t)(result % (seq_t)i);
        uint64_t g = a002458_gcd64((uint64_t)i, r);
        uint64_t d = (uint64_t)i / g;
        result = (result / (seq_t)g) * (seq_t)(num / d);
    }
    return result;
}
