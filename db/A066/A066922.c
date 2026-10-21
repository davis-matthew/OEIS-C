/*
 * A066922  a(n) = gcd(Omega(n), omega(n)).
 * https://oeis.org/A066922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(Ω(n), ω(n)) (Ω = total prime factor count, ω =
 *            distinct prime factor count)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n >= 1 by definition, but handle n < 2 for safety */
    if (n < (idx_t)2)
        return (seq_t)0;

    uint64_t x = (uint64_t)n;
    seq_t omega = (seq_t)0; /* distinct prime factors */
    seq_t Omega = (seq_t)0; /* total prime factors (with multiplicity) */

    /* trial division up to sqrt(x) */
    for (uint64_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            ++omega;
            uint64_t exp = 0;
            while (x % p == 0)
            {
                x /= p;
                ++exp;
                ++Omega;
            }
        }
    }

    /* if remainder > 1, it is a prime factor > sqrt(original n) */
    if (x > 1)
    {
        ++omega;
        ++Omega;
    }

    return gcd_seq_t(Omega, omega);
}
