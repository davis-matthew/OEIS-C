/*
 * A036158  3^n mod 199.
 * https://oeis.org/A036158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 199
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute 3^n modulo 199 using binary exponentiation.
       All intermediate values fit in seq_t because 0 <= result, base < 199. */
    const seq_t MOD = (seq_t)199;
    seq_t result = 1;
    seq_t base = 3 % MOD;
    idx_t exp = n;
    while (exp) {
        if (exp & 1)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}
