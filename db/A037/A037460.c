/*
 * A037460  a(n) = Sum{d(i)*6^i: i=0,1,...,m}, where Sum{d(i)*4^i:
 *          i=0,1,...,m} is the base 4 representation of n.
 * https://oeis.org/A037460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i≥0} d_i * 6^i where n = Σ_{i≥0} d_i * 4^i (base‑4
 *            digits d_i)
 * Range:     u8 n <= 83, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint8_t */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t pow6   = 1;
    idx_t t      = n;

    while (t) {
        uint8_t digit = t % 4;
        if (digit)
            result += pow6 * (seq_t)digit;
        pow6 *= (seq_t)6;
        t /= 4;
    }

    return result;
}
