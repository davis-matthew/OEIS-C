/*
 * A123484  Expansion of eta(q)^2 * eta(q^6)^4 * eta(q^8) * eta(q^24) /
 *          (eta(q^2) * eta(q^3) * eta(q^12))^2 in powers of q.
 * https://oeis.org/A123484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative with a(2) = –2, a(2^e) = 0 for e > 1
 *            a(3^e) = 1 a(p^e) = e + 1 if p ≡ 1 (mod 6) a(p^e) = (1 +
 *            (–1)^e)/2 if p ≡ 5 (mod 6)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset behaviour is irrelevant */
    seq_t result = 1;
    idx_t m = n;

    /* handle the factor 2 */
    if ((m & 1ULL) == 0) {
        unsigned exp2 = 0;
        while ((m & 1ULL) == 0) {
            exp2++;
            m >>= 1;
        }
        if (exp2 == 1)
            result *= (seq_t)(-2);
        else
            return 0;               /* any higher power of 2 gives 0 */
    }

    /* factor the remaining odd part multiplicatively */
    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);
        unsigned exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }

        seq_t factor;
        if (p == 3) {
            factor = 1;
        } else if ((p % 6) == 1) {
            factor = (seq_t)(exp + 1);
        } else if ((p % 6) == 5) {
            if ((exp & 1) == 0)
                factor = 1;
            else
                return 0;           /* odd exponent on a 5‑mod‑6 prime gives 0 */
        } else {
            /* p cannot be 2 here, and other primes are either 3 or ≡ 1,5 (mod 6) */
            factor = 1;
        }
        result *= factor;
    }

    return result;
}
