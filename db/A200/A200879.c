/*
 * A200879  Numbers n such that 10^n - 1 divides 10^(10^100) - 10.
 * https://oeis.org/A200879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = n such that 10^n - 1 divides 10^(10^100) - 10, which is
 *            equivalent to 10^100 ≡ 1 (mod n) for n > 1
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_mul.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    idx_t count = 1;          // divisor 1 already counted
    idx_t candidate = 2;      // start checking from 2

    while (1) {
        uint64_t mod   = candidate;   // modulo value fits in uint64_t
        uint64_t result = 1 % mod;    // current power result
        uint64_t base   = 10 % mod;   // base for modular exponentiation
        int exp = 100;                // exponent 100

        /* fast modular exponentiation using mod_mul to avoid overflow */
        while (exp) {
            if (exp & 1) result = mod_mul(result, base, mod);
            base = mod_mul(base, base, mod);
            exp >>= 1;
        }

        if (result == 1) {            // candidate divides 10^100 - 1
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }
}
