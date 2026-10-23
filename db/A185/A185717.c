/*
 * A185717  Expansion of q^(-1) * c(q^2) * (c(q) - c(q^4)) / 9 in powers of
 *          q^2 where c() is a cubic AGM theta function.
 * https://oeis.org/A185717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(2*n + 1) where b is multiplicative. * b(2^e) = 0^e ,
 *            b(3^e) = 3^e , b(p^e) = (p^(e+1)-1)/(p-1) for p>3
 * Range:     u8 n <= 7, u16 n <= 328, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer power: base^exp (exp fits in uint64_t) */
static inline seq_t pow_uint64(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* contribution of prime power p^e to the multiplicative function b. */
static inline seq_t factor_contrib(idx_t p, uint64_t exp)
{
    if (p == 3)
        return pow_uint64((seq_t)p, exp);

    /* (p^(e+1)-1)/(p-1) for p>3 */
    seq_t powp = pow_uint64((seq_t)p, exp + 1);
    seq_t numerator = powp;
    numerator -= 1;
    seq_t denom = (seq_t)(p - 1);
    return numerator / denom;
}

/* OEIS A185717 */
seq_t a(idx_t n)
{
    idx_t m = 2 * n + 1;           /* argument of b */
    seq_t result = 1;
    idx_t temp = m;

    /* trial division over odd primes (2 is irrelevant, m is odd) */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result *= factor_contrib(p, exp);
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        result *= factor_contrib(temp, 1);
    }

    return result;
}
