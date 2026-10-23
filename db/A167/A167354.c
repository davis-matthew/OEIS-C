/*
 * A167354  Totally multiplicative sequence with a(p) = (p-2)^2 = p^2-4p+4 for
 *          prime p.
 * https://oeis.org/A167354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e∥n} (p-2)^{2e} (multiplicative, a(p)=(p-2)^2,
 *            a(2k)=0)
 * Range:     u8 n <= 18, u16 n <= 262, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* compute base^exp for seq_t type using binary exponentiation */
static seq_t pow_seq(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;              /* a(1)=1 */

    if ((n & 1ULL) == 0ULL)           /* even numbers contain factor 2 → 0 */
        return (seq_t)0;

    uint64_t m = n;
    seq_t result = 1;

    /* remove factor 3 separately (could be any odd prime) */
    if (m % 3ULL == 0ULL)
    {
        uint64_t e = 0;
        while (m % 3ULL == 0ULL)
        {
            m /= 3ULL;
            ++e;
        }
        seq_t base = (seq_t)(3ULL - 2ULL) * (seq_t)(3ULL - 2ULL); /* (p-2)^2 */
        result *= pow_seq(base, e);
    }

    /* trial division by odd numbers starting from 5, skipping multiples of 3
       (increment by 6: 5,11,17,23,…) */
    for (uint64_t p = 5ULL; p <= m / p; p += 6ULL)
    {
        /* check p */
        if (m % p == 0ULL)
        {
            uint64_t e = 0;
            while (m % p == 0ULL)
            {
                m /= p;
                ++e;
            }
            seq_t base = (seq_t)(p - 2ULL) * (seq_t)(p - 2ULL);
            result *= pow_seq(base, e);
        }

        uint64_t q = p + 2ULL;         /* the next candidate (p+2) is coprime to 3 */
        if (q > m / q)                 /* break if q > sqrt(m) */
            break;
        if (m % q == 0ULL)
        {
            uint64_t e = 0;
            while (m % q == 0ULL)
            {
                m /= q;
                ++e;
            }
            seq_t base = (seq_t)(q - 2ULL) * (seq_t)(q - 2ULL);
            result *= pow_seq(base, e);
        }
    }

    /* if remaining m > 1, it is a prime factor > sqrt(original n) */
    if (m > 1ULL)
    {
        seq_t base = (seq_t)(m - 2ULL) * (seq_t)(m - 2ULL);
        result *= base;                /* exponent is 1 */
    }

    return result;
}
