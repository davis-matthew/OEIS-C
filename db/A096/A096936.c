/*
 * A096936  Half of number of integer solutions to the equation x^2 + 3y^2 =
 *          n.
 * https://oeis.org/A096936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A033716(n) / 2. * A033716 counts the representations of n
 *            as x^2 + 3y^2. * The sequence is multiplicative with the
 *            following local rules: * - a(2^e) = 3*(1+(-1)^e)/2: 0 for odd e,
 *            3 for even e. * - a(3^e) = 1 for all e. * - a(p^e) (p ≡ 1 mod 3)
 *            = 1 + e. * - a(p^e) (p ≡ 2 mod 3) = (1+(-1)^e...
 * Range:     u8 n <= 9978, u16 n <= 9978, u32 n <= 9978, u64 n <= 9978,
 *            u128 n <= 9978, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

extern seq_t a(idx_t n)
{
    idx_t m = n;
    seq_t result = 1;          /* start with multiplicative identity */

    /* factor out powers of 2 */
    uint64_t e2 = 0;
    while ((m & 1ULL) == 0) {
        e2++;
        m >>= 1;
    }
    if (e2) {
        if (e2 & 1ULL)         /* odd exponent → contribution 0 */
            return 0;
        result *= (seq_t)3;     /* even exponent → factor 3 */
    }

    /* factor out powers of 3 */
    uint64_t e3 = 0;
    while (m % 3ULL == 0) {
        e3++;
        m /= 3ULL;
    }
    /* a(3^e) = 1, so nothing to multiply */

    /* factor remaining primes */
    for (uint64_t p = 5; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                e++;
                m /= p;
            }
            if (p % 3ULL == 1ULL) {            /* p ≡ 1 (mod 3) */
                result *= (seq_t)(1 + e);       /* contribution 1+e */
            } else {                           /* p ≡ 2 (mod 3) */
                if (e & 1ULL)                  /* odd exponent → 0 */
                    return 0;
                /* even exponent → factor 1, no change */
            }
        }
    }

    /* m now is 1 or a prime > sqrt(original n) */
    if (m > 1) {
        uint64_t p = m;
        uint64_t e = 1;                       /* exponent is 1 */
        if (p % 3ULL == 1ULL) {
            result *= (seq_t)(1 + e);          /* 1+1 = 2 */
        } else {                              /* p ≡ 2 (mod 3) */
            if (e & 1ULL)                      /* odd exponent → 0 */
                return 0;
            /* even exponent (1) never occurs, but handled for completeness */
        }
    }

    return result;
}
