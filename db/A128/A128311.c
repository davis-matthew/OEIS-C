/*
 * A128311  Remainder upon division of 2^(n-1)-1 by n.
 * https://oeis.org/A128311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(n-1) - 1) mod n
 * Range:     u8 n <= 323, u16 n <= 2048, u32 n <= 2048, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            // safety for out‑of‑range input
    uint64_t mod  = (uint64_t)n;            // modulus is n itself
    uint64_t exp  = (uint64_t)(n - 1);      // exponent is n-1
    uint64_t result = 1 % mod;              // current value of 2^k mod n
    uint64_t base   = 2 % mod;              // base = 2

    // fast exponentiation: compute 2^(n-1) mod n
    while (exp > 0) {
        if (exp & 1)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1;
    }

    // subtract 1 and reduce modulo n again
    uint64_t rem = (result + mod - 1) % mod;
    return (seq_t)rem;
}
