/*
 * A214361  Expansion of c(q^2) * (c(q) + 2 * c(q^4)) / 9 in powers of q where
 *          c() is a cubic AGM theta function.
 * https://oeis.org/A214361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative definition from OEIS A214361 * * a(1) = 1 *
 *            a(2^e) = 3 (e > 0) * a(3^e) = 3^e * a(p^e) = (p^(e+1) – 1) / (p
 *            – 1) for primes p > 3 * * The function factors n using the
 *            provided helper * uint64_t smallest_prime_factor(uint64_t n) *
 *            (declared static in the header) and then evaluates e...
 * Range:     u8 n <= 16, u16 n <= 586, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/smallest_prime_factor.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;            /* offset 1 */

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);    /* smallest prime factor */
        uint64_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        seq_t part;
        if (p == 2) {
            part = (seq_t)3;                       /* a(2^e) = 3 */
        } else if (p == 3) {
            part = big_pow((seq_t)3, (seq_t)exp);  /* a(3^e) = 3^e */
        } else {
            seq_t top = big_pow((seq_t)p, (seq_t)(exp + 1));
            seq_t bottom = (seq_t)p - (seq_t)1;
            part = (top - (seq_t)1) / bottom;      /* (p^(e+1)-1)/(p-1) */
        }

        result *= part;
    }
    return result;
}
