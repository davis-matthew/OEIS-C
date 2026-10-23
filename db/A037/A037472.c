/*
 * A037472  a(n)=Sum{d(i)*9^i: i=0,1,...,m}, where Sum{d(i)*6^i: i=0,1,...,m}
 *          is the base 6 representation of n.
 * https://oeis.org/A037472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i>=0} d_i * 9^i, where n = Σ_{i>=0} d_i * 6^i (base‑6
 *            representation)
 * Range:     u8 n <= 117, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t pow9 = 1;
    while (n > 0) {
        uint64_t digit = n % 6;
        res += (seq_t)digit * pow9;
        pow9 *= 9;
        n /= 6;
    }
    return res;
}
