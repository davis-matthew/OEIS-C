/*
 * A036134  a(n) = 3^n mod 79.
 * https://oeis.org/A036134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 79
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    const uint64_t MOD = 79ULL;
    uint64_t res   = 1;          /* result */
    uint64_t base  = 3;          /* base */
    idx_t exp = n;               /* exponent */
    while (exp > 0) {
        if (exp & 1UL)
            res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return (seq_t)res;
}
