/*
 * A003961  Completely multiplicative with a(prime(k)) = prime(k+1).
 * https://oeis.org/A003961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, nice
 * Formula:   If n = Product p(k)^e(k) then a(n) = Product p(k+1)^e(k)
 *            (completely multiplicative, a(prime(k)) = prime(k+1))
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 8852 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= (seq_t)q;
            }
        }
    }
    if (m > 1) {
        result *= (seq_t)next_prime(m);
    }
    return result;
}
