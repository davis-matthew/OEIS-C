/*
 * A007581  a(n) = (2^n+1)*(2^n+2)/6.
 * https://oeis.org/A007581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2^n+1)*(2^(n-1)+1)/3 for n >= 1, a(0) = 1 (equal to
 *            (2^n+1)*(2^n+2)/6)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t p = pw2(n) + 1;
    seq_t q = pw2(n - 1) + 1;
    if (n % 2 == 1) {
        p /= 3;
    } else {
        q /= 3;
    }
    return p * q;
}
