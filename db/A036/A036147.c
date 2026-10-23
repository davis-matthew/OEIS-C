/*
 * A036147  a(n) = 2^n mod 149.
 * https://oeis.org/A036147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 149
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t base = 2 % 149;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % 149;
        }
        base = (base * base) % 149;
        exp >>= 1;
    }
    return result;
}
