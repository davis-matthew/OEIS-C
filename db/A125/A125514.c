/*
 * A125514  Theta series of 4-dimensional lattice QQF.4.i.
 * https://oeis.org/A125514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * b(n) where b is multiplicative: * b(2^e) = 2^(e+2) -
 *            3 * b(3^e) = 1 * b(p^e) = (p^(e+1) - 1) / (p - 1) for all other
 *            primes p Initial values: a(0)=1, a(1)=4, a(2)=20, a(3)=4, …
 * Range:     u8 n <= 16, u16 n <= 2401, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>


/* simple exponentiation by squaring for the unsigned integer type seq_t */
static inline seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
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
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* factor n by simple trial division (fast enough for n up to ≈10⁴) */
    idx_t nn = n;
    uint64_t primes[64];
    size_t  exps[64];
    size_t  cnt = 0;

    for (idx_t p = 2; (unsigned long long)p * (unsigned long long)p <= nn; ++p)
    {
        if (nn % p == 0)
        {
            size_t e = 0;
            while (nn % p == 0)
            {
                nn /= p;
                ++e;
            }
            primes[cnt] = (uint64_t)p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (nn > 1)
    {
        primes[cnt] = nn;
        exps[cnt]   = 1;
        ++cnt;
    }

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        size_t   e = exps[i];
        seq_t term;

        if (p == 2)
        {
            term = pow_uint((seq_t)2, (idx_t)(e + 2));
            term -= (seq_t)3;
        }
        else if (p == 3)
        {
            term = 1;
        }
        else
        {
            seq_t numer = pow_uint((seq_t)p, (idx_t)(e + 1));
            numer -= (seq_t)1;
            seq_t denom = (seq_t)(p - 1);
            term = numer / denom;
        }

        prod *= term;
    }

    return prod * (seq_t)4;        /* a(n) = 4 * b(n) */
}
