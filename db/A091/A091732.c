/*
 * A091732  Iphi(n): infinitary analog of Euler's phi function.
 * https://oeis.org/A091732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e||n} Π_{j: 2^j bit of e is 1} (p^(2^j) - 1) That
 *            is, a(n) = product of (q – 1) for each distinct i-divisor q of n
 * Range:     u8 n <= 256, u16 n <= 9958, u32 n <= 9958, u64 n <= 9958,
 *            u128 n <= 9958, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast exponentiation for seq_t (unsigned) base and unsigned long long exponent */
static inline seq_t pow_seq_t(seq_t base, unsigned long long exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1)           /* a(1) = 1 */
        return (seq_t)1;

    idx_t m = n;
    seq_t res = 1;

    /* trial division up to sqrt(m) */
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p != 0)
            continue;

        /* exponent of prime p in n */
        idx_t e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }

        /* process each set bit in e */
        idx_t bitpos = 0;
        idx_t tmp = e;
        while (tmp)
        {
            if (tmp & 1)
            {
                seq_t q = pow_seq_t((seq_t)p, 1ULL << bitpos);
                res *= (q - 1);
            }
            tmp >>= 1;
            ++bitpos;
        }
    }

    /* remaining prime factor > sqrt with exponent 1 */
    if (m > 1)
    {
        seq_t q = pow_seq_t((seq_t)m, 1ULL);
        res *= (q - 1);
    }

    return res;
}
