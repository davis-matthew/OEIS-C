/*
 * A006571  Expansion of q*Product_{k>=1} (1-q^k)^2*(1-q^(11*k))^2.
 * https://oeis.org/A006571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice, mult
 * Formula:   a(n) is multiplicative with a(11^e) = 1, a(p^e) =
 *            a(p)*a(p^(e-1)) - p*a(p^(e-2)) for p != 11, where a(p) = p -
 *            #{(x,y) in F_p^2 : y^2 + y = x^3 - x^2} (elliptic curve 11a3)
 * Range:     i8 n <= 2653, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   a_prime_power, ap_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a_prime_power.h"
#include "../utils/ap_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    long long result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            result *= a_prime_power(p, e);
            if (result == 0) return (seq_t)0;
        }
    }
    if (m > 1) result *= a_prime_power(m, 1);
    return (seq_t)result;
}
