/*
 * A001756  a(n) = A059366(n,n-3) = A059366(n,3) for n >= 3, where the
 *          triangle A059366 arises from the expansion of a trigonometric
 *          integral.
 * https://oeis.org/A001756
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*n*(2*n-7)/(n-3), a(3) = 15
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   a001756_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001756_gcd.h"

seq_t a(idx_t n) {
    seq_t result = 15;
    for (idx_t i = 4; i <= n; i++) {
        idx_t d = i - 3;
        idx_t f1 = i;
        idx_t f2 = 2 * i - 7;
        idx_t g = a001756_gcd(d, f1);
        d /= g;
        f1 /= g;
        g = a001756_gcd(d, f2);
        d /= g;
        f2 /= g;
        result /= (seq_t)d;
        result *= (seq_t)f1;
        result *= (seq_t)f2;
    }
    return result;
}
