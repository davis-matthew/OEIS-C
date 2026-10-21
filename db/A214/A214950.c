/*
 * A214950  Denominator of sum of reciprocals of all nonzero digits of n in
 *          decimal representation.
 * https://oeis.org/A214950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   Denominator of sum of reciprocals of all nonzero digits of n in
 *            decimal representation
 * Range:     u8 n <= 577, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t num = (seq_t)0;
    seq_t den = (seq_t)1;
    idx_t x = n;
    while (x > 0) {
        uint8_t d = (uint8_t)(x % 10);
        x /= 10;
        if (d == 0) continue;
        seq_t d_seq = (seq_t)d;
        seq_t g = gcd(den, d_seq);
        seq_t l = den / g * d_seq;          // LCM of current denominator and digit
        seq_t term_num = l / d_seq;         // 1/d expressed over the common denominator
        seq_t num_scaled = num * (l / den);
        num = num_scaled + term_num;
        den = l;
        seq_t g2 = gcd(num, den);
        if (g2 > (seq_t)1) {
            num /= g2;
            den /= g2;
        }
    }
    return den;
}
