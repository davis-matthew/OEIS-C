/*
 * A003143  a(2*n) = floor( 17*2^n/14 ), a(2*n+1) = floor( 12*2^n/7 ).
 * https://oeis.org/A003143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = floor(17*2^n/14), a(2*n+1) = floor(12*2^n/7); computed
 *            as 2^n = 14*q + r => a(2n) = 17*q + floor(17*r/14), and
 *            similarly with 7 for odd n, so no intermediate value exceeds the
 *            result
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   a3143_p2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a3143_p2.h"

seq_t a(idx_t n) {
    idx_t k = n / 2;
    seq_t p = a3143_p2(k);
    if (n % 2 == 0) {
        seq_t q = p / 14;
        long long r = (long long)(p % 14);
        return (seq_t)17 * q + (seq_t)((17 * r) / 14);
    } else {
        seq_t q = p / 7;
        long long r = (long long)(p % 7);
        return (seq_t)12 * q + (seq_t)((12 * r) / 7);
    }
}
