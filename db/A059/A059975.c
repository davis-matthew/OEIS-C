/*
 * A059975  For n > 1, a(n) is the least number of prime factors (counted with
 *          multiplicity) of any integer with n divisors; fully additive with
 *          a(p) = p-1.
 * https://oeis.org/A059975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ e_i·(p_i − 1) over the prime factorization of n
 * Range:     u8 n <= 256, u16 n <= 8109, u32 n <= 8109, u64 n <= 8109,
 *            u128 n <= 8109, big any n
 * Helpers:   add_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0; for n > 1, a(n) = sum over p^e||n of e*(p-1). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/add_term.h"

/* Main function */
seq_t a(idx_t n) {
    if (n <= 1) {
        return (seq_t)0;
    }
    seq_t result = (seq_t)0;
    idx_t temp = n;

    /* factor out 2 separately */
    if ((temp & 1ULL) == 0) {
        idx_t cnt = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            cnt++;
        }
        result += add_term(cnt, 2);
    }

    /* trial division by odd primes up to sqrt(temp) */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                cnt++;
            }
            result += add_term(cnt, p);
        }
    }

    /* remaining temp is 1 or a prime > sqrt(original n) */
    if (temp > 1) {
        /* single factor with exponent 1 */
        result += add_term(1, temp);
    }

    return result;
}
