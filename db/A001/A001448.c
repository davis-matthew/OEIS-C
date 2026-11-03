/*
 * A001448  a(n) = binomial(4n,2n) or (4*n)!/((2*n)!*(2*n)!).
 * https://oeis.org/A001448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = binomial(4n,2n) = Product_{i=1..2n} (2n+i)/i
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 100 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n) {
    idx_t k = 2 * n;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; i++) {
        /* result * (k+i) / i is exact (it equals binomial(k+i, i)).
           Remove gcd(k+i, i) first; then i/g divides result exactly. */
        uint64_t num = (uint64_t)(k + i);
        uint64_t den = (uint64_t)i;
        uint64_t g = gcd_u64(num, den);
        num /= g;
        den /= g;
        result /= (seq_t)den;
        result *= (seq_t)num;
    }
    return result;
}
