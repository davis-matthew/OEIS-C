/*
 * A004986  a(n) = (2^n/n!) * Product_{k=0..n-1} (4*k + 7).
 * https://oeis.org/A004986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(4*n+3)*a(n-1)/n, a(0) = 1 (i.e. a(n) = (2^n/n!) *
 *            Product_{k=0..n-1} (4*k+7))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   a004986_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004986_gcd_u64.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        uint64_t m = (uint64_t)i;
        uint64_t t = 2 * (4 * (uint64_t)i + 3);
        /* cancel common factor of result and i */
        uint64_t r = (uint64_t)(result % (seq_t)m);
        uint64_t g1 = a004986_gcd_u64(m, r);
        result /= (seq_t)g1;
        m /= g1;
        /* cancel common factor of t and remaining divisor */
        uint64_t g2 = a004986_gcd_u64(t, m);
        t /= g2;
        m /= g2;
        /* now m is coprime to t, so m divides result */
        result /= (seq_t)m;
        result *= (seq_t)t;
    }
    return result;
}
