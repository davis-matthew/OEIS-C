/*
 * A004381  Binomial coefficient C(8n,n).
 * https://oeis.org/A004381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(8n,n) = Product_{i=1..n} (7n+i)/i
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   a004381_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004381_gcd_u64.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        uint64_t num = (uint64_t)(7 * n + i);
        uint64_t ri = (uint64_t)(long long)(r % (seq_t)i);
        uint64_t g = a004381_gcd_u64((uint64_t)i, ri);
        uint64_t den = (uint64_t)i / g;
        r /= (seq_t)g;
        r *= (seq_t)(num / den);
    }
    return r;
}
