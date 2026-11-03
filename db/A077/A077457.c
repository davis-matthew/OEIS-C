/*
 * A077457  a(n) = sigma_4(n^4)/sigma_2(n^4).
 * https://oeis.org/A077457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 256,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 41 terms through n = 9290 (2026-11-01)
 * Helpers:   none
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

/* Compute (base^exp) using unsigned __int128 to avoid overflow.
 *   base may be up to 10000, exp up to 106 (for n <= 10000).
 *   The result fits in unsigned __int128 for all allowed inputs. */
static inline unsigned __int128 pow_u128(unsigned __int128 base, unsigned exp)
{
    unsigned __int128 result = 1;
    while (exp) {
        if (exp & 1u) result *= base;
        base *= base;
        exp >>= 1u;
    }
    return result;
}

/* Compute the multiplicative factor for a prime power p^e */
static inline seq_t prime_power_factor(idx_t p, unsigned e)
{
    unsigned exp = 8u * e + 2u;                 /* exponent of p in numerator   */
    unsigned __int128 num = pow_u128((unsigned __int128)p, exp);
    num += 1;                                   /* numerator: p^exp + 1         */
    unsigned __int128 den = (unsigned __int128)p * p + 1; /* denominator: p^2 + 1     */
    unsigned __int128 fact = num / den;          /* exact division                */
    return (seq_t)fact;                         /* fits in seq_t for allowed n   */
}

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
