/*
 * A011619  Legendre symbol (n,181).
 * https://oeis.org/A011619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = n^90 (mod 181), where the residue 180 is interpreted as
 *            -1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* helper: modular exponentiation for uint64_t */
static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1u) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1u;
    }
    return result;
}

seq_t a(idx_t n) {
    const uint64_t MOD = 181ULL;
    /* Legendre symbol via Euler's criterion */
    uint64_t res = mod_pow_uint64((uint64_t)n, 90ULL, MOD);
    if (res == MOD - 1u) {
        return (seq_t)-1;
    }
    return (seq_t)res; /* 0 or 1 */
}
