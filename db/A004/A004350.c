/*
 * A004350  Binomial coefficient C(5n,n-8).
 * https://oeis.org/A004350
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(5*n, n-8), computed multiplicatively as
 *            prod_{i=1..n-8} (5*n-i+1)/i with gcd reduction
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 15, u64 n <= 23, u128 n <= 26,
 *            big any n
 * Helpers:   a004350_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004350_gcd_u64.h"

seq_t a(idx_t n) {
    idx_t k = n - 8;
    seq_t result = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        /* result * m / i is exact; reduce first to avoid overflow */
        seq_t m = (seq_t)(5 * n - i + 1);
        uint64_t r = (uint64_t)(result % (seq_t)i);
        uint64_t g = a004350_gcd_u64(r, (uint64_t)i);
        if (g == 0) g = (uint64_t)i;
        uint64_t d = (uint64_t)i / g;
        result /= (seq_t)g;
        m /= (seq_t)d;
        result *= m;
    }
    return result;
}
