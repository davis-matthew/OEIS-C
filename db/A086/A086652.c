/*
 * A086652  a(n) = A000225(n+3)-A052955(n).
 * https://oeis.org/A086652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+3)-((5-(-1)^n)/2)*2^((2*n-1+(-1)^n)/4)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    seq_t total = pow2(n + 3);            /* 2^(n+3) */
    idx_t k = (n & 1ULL) ? (n - 1) / 2 : n / 2;
    seq_t sub = seq_mul(pow2(k), (seq_t)((n & 1ULL) ? 3 : 2));
    return total - sub;
}
