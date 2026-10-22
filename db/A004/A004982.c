/*
 * A004982  a(n) = (2^n/n!) * Product_{k=0..n-1} (4*k + 3).
 * https://oeis.org/A004982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(4*n-1)*a(n-1)/n, a(0) = 1 (i.e. a(n) = (2^n/n!) *
 *            Product_{k=0..n-1} (4*k + 3))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   a004982_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004982_gcd_u64.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        /* result * 2*(4i-1) is divisible by i; cancel common factors first
           so the intermediate never exceeds the final value a(i). */
        uint64_t rm = (uint64_t)(result % (seq_t)i);
        uint64_t g = a004982_gcd_u64((uint64_t)i, rm);
        uint64_t i2 = (uint64_t)i / g;
        uint64_t m = 2 * (4 * (uint64_t)i - 1) / i2;
        result /= (seq_t)g;
        result *= (seq_t)m;
    }
    return result;
}
