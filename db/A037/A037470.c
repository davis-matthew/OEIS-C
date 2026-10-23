/*
 * A037470  a(n)=Sum{d(i)*7^i: i=0,1,...,m}, where Sum{d(i)*6^i: i=0,1,...,m}
 *          is the base 6 representation of n.
 * https://oeis.org/A037470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ d_i(n)*7^i, where n = Σ d_i(n)*6^i (digits d_i in base
 *            6)
 * Range:     u8 n <= 189, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t pow7 = 1;
    while (n > 0) {
        idx_t digit = n % 6;
        if (digit) {
            result += (seq_t)digit * pow7;
        }
        pow7 *= (seq_t)7;
        n /= 6;
    }
    return result;
}
