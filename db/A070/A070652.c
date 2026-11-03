/*
 * A070652  a(n) = n^6 mod 29.
 * https://oeis.org/A070652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 29
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 80 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Reduce n modulo 29 first */
    seq_t base = (seq_t)(n % 29);
    seq_t result = 1;
    uint64_t exp = 6;          /* exponent is constant 6 */
    while (exp) {
        if (exp & 1)
            result = (result * base) % 29;
        base = (base * base) % 29;
        exp >>= 1;
    }
    return result;
}
