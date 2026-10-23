/*
 * A133693  Expansion of (1 - phi(-q) * phi(q^2)) / 2 in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A133693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = ∏_{p^e||n} f(p,e) where f(2,e) = -1 (e>0) f(p,e) for
 *            p≡1,3 (mod 8) = e+1 f(p,e) for p≡5,7 (mod 8) = 1 if e even, 0 if
 *            e odd If n contains a prime factor p≡5 or 7 (mod 8) with odd
 *            exponent, the whole product is zero
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static seq_t prime_power_value(uint64_t p, idx_t e)
{
    if (p == 2ULL)        /* 2‑power */
        return (seq_t)-1; /* e > 0 */

    uint64_t mod8 = p & 7ULL;
    if (mod8 == 5ULL || mod8 == 7ULL)
        return (e & 1ULL) ? (seq_t)0 : (seq_t)1;
    /* mod8 == 1 or 3 */
    return (seq_t)(e + 1);
}

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return 0; /* undefined, but safe fallback */

    seq_t result = 1;
    idx_t temp = n;

    /* factor 2 */
    if ((temp & 1ULL) == 0ULL) {
        idx_t e = 0;
        while ((temp & 1ULL) == 0ULL) {
            temp >>= 1;
            ++e;
        }
        result *= prime_power_value(2ULL, e);
    }

    /* factor odd primes */
    for (idx_t p = 3ULL; p * p <= temp; p += 2ULL) {
        if (temp % p == 0ULL) {
            idx_t e = 0;
            while (temp % p == 0ULL) {
                temp /= p;
                ++e;
            }
            result *= prime_power_value(p, e);
            if (result == (seq_t)0) /* zero propagates, can stop early */
                return 0;
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (temp > 1ULL) {
        idx_t e = 1;
        result *= prime_power_value(temp, e);
    }

    return result;
}
