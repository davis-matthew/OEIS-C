/*
 * A038994  Number of sublattices of index n in generic 7-dimensional lattice.
 * https://oeis.org/A038994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative with a(p^e) = Π_{k=1..6}
 *            (p^(e+k)-1)/(p^k-1) (Gaussian binomial coefficient [e+6 choose
 *            6]_p) a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 24, u64 n <= 1199,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/gcd.h"

static seq_t a_prime_power(uint64_t p, uint64_t e)
{
    seq_t num = 1;   /* numerator of the product */
    seq_t den = 1;   /* denominator of the product */

    for (uint64_t k = 1; k <= 6; ++k) {
        seq_t num_k = big_pow((seq_t)p, (seq_t)(e + k));
        num_k -= 1;
        seq_t den_k = big_pow((seq_t)p, (seq_t)k);
        den_k -= 1;

        /* cancel num_k with den, and den_k with num, before multiplication
           to keep intermediate values reasonably small */
        seq_t g1 = gcd(num_k, den);
        if (g1 > 1) {
            num_k /= g1;
            den   /= g1;
        }
        seq_t g2 = gcd(den_k, num);
        if (g2 > 1) {
            den_k /= g2;
            num   /= g2;
        }

        num *= num_k;
        den *= den_k;

        /* final reduction */
        seq_t g3 = gcd(num, den);
        if (g3 > 1) {
            num /= g3;
            den /= g3;
        }
    }
    /* den should be 1, but we divide to produce an integer result */
    return num / den;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined, but return 0 for safety */
    if (n == 1) return (seq_t)1;     /* base case */

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* factor 2 */
    if (m % 2 == 0) {
        uint64_t e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        result *= a_prime_power(2, e);
    }

    /* odd factors */
    for (uint64_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) {
            uint64_t e = 0;
            while (m % d == 0) {
                m /= d;
                ++e;
            }
            result *= a_prime_power(d, e);
        }
    }

    /* remaining prime factor */
    if (m > 1) {
        result *= a_prime_power(m, 1);
    }

    return result;
}
