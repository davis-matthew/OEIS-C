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
 * Helpers:   a3961_nextprime, a3961_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a3961_nextprime.h"
#include "../utils/a3961_prime.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = a3961_nextprime(p);
            while (m % p == 0) {
                m /= p;
                result *= (seq_t)q;
            }
        }
    }
    if (m > 1) {
        result *= (seq_t)a3961_nextprime(m);
    }
    return result;
}
