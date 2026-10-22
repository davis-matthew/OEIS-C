/*
 * A006134  a(n) = Sum_{k=0..n} binomial(2*k,k).
 * https://oeis.org/A006134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*k,k), using C(2k,k) =
 *            C(2k-2,k-1)*(4k-2)/k, C(0,0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   a006134_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006134_gcd.h"

seq_t a(idx_t n) {
    seq_t c = 1;      /* C(2k,k) */
    seq_t result = 1; /* k = 0 term */
    for (idx_t k = 1; k <= n; k++) {
        /* c = c * (4k-2) / k, done without exceeding the new value */
        uint64_t r = (uint64_t)(c % (seq_t)k);
        uint64_t g = a006134_gcd(k, r);   /* gcd(c, k) */
        uint64_t kk = k / g;              /* coprime to c/g, so divides 4k-2 */
        uint64_t m = (4 * (uint64_t)k - 2) / kk;
        c = (c / (seq_t)g) * (seq_t)m;
        result += c;
    }
    return result;
}
