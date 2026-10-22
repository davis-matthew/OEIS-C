/*
 * A001311  Final 2 digits of 6^n.
 * https://oeis.org/A001311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n mod 100
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t result = 1 % 100;
    uint64_t base = 6;
    idx_t e = n;
    while (e > 0) {
        if (e & 1) result = (result * base) % 100;
        base = (base * base) % 100;
        e >>= 1;
    }
    return (seq_t)result;
}
