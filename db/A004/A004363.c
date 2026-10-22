/*
 * A004363  Binomial coefficient C(6n,n-8).
 * https://oeis.org/A004363
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(6*n, n-8) = Product_{i=1..n-8} (5*n+8+i)/i
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 14, u64 n <= 22, u128 n <= 26,
 *            big any n
 * Helpers:   a004363_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004363_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 8;
    idx_t base = 5 * n + 8;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        /* r = C(base+i-1, i-1); next r = r * (base+i) / i, exactly.
           Reduce first so no intermediate exceeds the new value. */
        uint64_t rm = (uint64_t)(r % (seq_t)i);
        uint64_t g = a004363_gcd(rm, (uint64_t)i);
        if (g == 0) g = (uint64_t)i;
        uint64_t d = (uint64_t)i / g;           /* d divides base+i */
        uint64_t num = ((uint64_t)base + (uint64_t)i) / d;
        r = (r / (seq_t)g) * (seq_t)num;
    }
    return r;
}
