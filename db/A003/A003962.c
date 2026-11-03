/*
 * A003962  Completely multiplicative with a(prime(k)) = floor(
 *          (prime(k+1)+1)/2 ) for k-th prime prime(k).
 * https://oeis.org/A003962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   completely multiplicative with a(p^e) =
 *            ((A000040(A000720(p)+1)+1)/2)^e, i.e. a(p) =
 *            floor((nextprime(p)+1)/2)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9566 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

static seq_t a3962_prime_value(uint64_t p) {
    uint64_t q = next_prime(p);
    return (seq_t)((q + 1) / 2);
}

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; p++) {
        if (temp % p == 0) {
            seq_t v = a3962_prime_value((uint64_t)p);
            while (temp % p == 0) {
                result *= v;
                temp /= p;
            }
        }
    }

    if (temp > 1) {
        result *= a3962_prime_value((uint64_t)temp);
    }

    return result;
}
