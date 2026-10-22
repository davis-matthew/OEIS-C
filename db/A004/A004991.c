/*
 * A004991  a(n) = (3^n/n!) * Product_{k=0..n-1} (3*k + 4).
 * https://oeis.org/A004991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(3*n+1)*a(n-1)/n, a(0) = 1 (i.e. a(n) = (3^n/n!) *
 *            Product_{k=0..n-1} (3*k+4))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   g_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/g_u64.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        /* a(i) = a(i-1) * 3*(3i+1) / i is exact; divide out gcd(a(i-1), i) first
           so that i/g divides 3*(3i+1) and no intermediate exceeds a(i). */
        uint64_t r = (uint64_t)(result % (seq_t)i);
        uint64_t g = g_u64(i, r);
        uint64_t d = (uint64_t)i / g;
        seq_t m = (seq_t)3 * (3 * (seq_t)i + 1) / (seq_t)d;
        result = (result / (seq_t)g) * m;
    }
    return result;
}
