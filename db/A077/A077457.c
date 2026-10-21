/*
 * A077457  a(n) = sigma_4(n^4)/sigma_2(n^4).
 * https://oeis.org/A077457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 256,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_u128, prime_power_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A001159(n^4) / A001157(n^4)
 *          = ∏_{p^e || n} (p^(8e+2) + 1) / (p^2 + 1)
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow_u128.h"
#include "../utils/prime_power_factor.h"

/* Factorise n and accumulate the product of factors. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* safe fallback for unused idx */

    seq_t result = 1;
    idx_t temp   = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            result *= prime_power_factor(p, e);
        }
    }

    if (temp > 1) { /* remaining prime factor > sqrt(n) */
        result *= prime_power_factor(temp, 1);
    }

    return result;
}
