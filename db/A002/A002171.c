/*
 * A002171  Glaisher's chi numbers. a(n) = chi(4*n + 1).
 * https://oeis.org/A002171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = b(4*n+1), b multiplicative with b(p^e) = b(p)*b(p^(e-1))
 *            - p*b(p^(e-2)), b(1) = 1, b(p) = p - #{(x,y) mod p : y^2 = x^3 -
 *            x}
 * Range:     i8 n <= 383, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   b_prime, b_prime_power, powmod_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/b_prime.h"
#include "../utils/b_prime_power.h"
#include "../utils/powmod_small.h"

seq_t a(idx_t n) {
    uint64_t m = 4 * (uint64_t)n + 1;
    uint64_t p;
    seq_t result = 1;
    for (p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            result *= b_prime_power(p, e);
        }
    }
    if (m > 1) result *= b_prime_power(m, 1);
    return result;
}
