/*
 * A001159  sigma_4(n): sum of 4th powers of divisors of n.
 * https://oeis.org/A001159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma_4(n), multiplicative with a(p^e) = 1 + p^4 + p^8 +
 *            ... + p^(4e) = (p^(4e+4)-1)/(p^4-1)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 251, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma4_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma4_prime_power.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            result *= sigma4_prime_power(p, e);
        }
    }

    if (temp > 1) {
        result *= sigma4_prime_power(temp, 1);
    }

    return result;
}
