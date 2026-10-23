/*
 * A133657  Expansion of q * (phi(q) * psi(q^4))^2 in powers of q where phi(),
 *          psi() are Ramanujan theta functions.
 * https://oeis.org/A133657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   multiplicative function a(n) defined by a(2^e * m) = 0 if e >= 2
 *            a(m) = sigma(m) if e = 0 (m odd) a(2*m) = 4 * sigma(m) if e = 1
 *            (m odd)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sigma(m) = sum of divisors of m */

#include <stdint.h>
#include <stdbool.h>

/* helper: sigma of a single prime power p^e (p odd) */
static seq_t sigma_prime_power(uint64_t p, int e)
{
    seq_t sum = 1;          /* term for p^0 */
    seq_t term = 1;         /* current p^i */
    for (int i = 1; i <= e; ++i) {
        term *= (seq_t)p;
        sum += term;
    }
    return sum;
}

/* helper: sigma of an odd number m */
static seq_t sigma_of_odd(uint64_t m)
{
    if (m == 1)
        return 1;

    seq_t result = 1;
    uint64_t temp = m;

    /* trial division over odd potential factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result *= sigma_prime_power(p, exp);
        }
    }

    /* if a prime factor remains > sqrt(original) */
    if (temp > 1) {
        /* remaining prime factor has exponent 1 */
        result *= sigma_prime_power(temp, 1);
    }

    return result;
}

seq_t a(idx_t n)
{
    /* n >= 1 according to OEIS offset */

    /* count trailing zeros in binary representation of n */
    int e = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        ++e;
    }

    /* if divisible by 4 -> result is 0 */
    if (e >= 2)
        return 0;

    uint64_t m = tmp;   /* odd part of n */

    seq_t sigma = sigma_of_odd(m);

    if (e == 0)          /* odd n */
        return sigma;
    else                 /* e == 1 : n = 2 * odd */
        return sigma * 4;
}
