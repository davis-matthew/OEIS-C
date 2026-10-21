/*
 * A236563  a(p^n)=p^(n+1)(p-1) if p is prime and a(nm)=lcm(a(n),a(m)) if
 *          gcd(n,m)=1.
 * https://oeis.org/A236563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm_{p^e‖n} (p^(e+1)*(p-1)) with a(1)=1 (p^e‖n means p^e
 *            is a maximal prime power divisor of n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   add_or_update, seq_mul, seq_pow, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/smallest_prime_factor.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/add_or_update.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    uint64_t temp = (uint64_t)n;

    /* arrays for lcm prime factors; maximum size needed is small (≤128). */
    uint64_t lpr[128];
    uint64_t lex[128];
    size_t lcnt = 0;

    /* factor n and accumulate contributions */
    while (temp > 1)
    {
        uint64_t p = smallest_prime_factor(temp);
        uint64_t e = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++e;
        }

        /* contribution of p^e : p^(e+1) */
        add_or_update(p, e + 1, lpr, lex, &lcnt);

        /* contribution of (p-1) */
        uint64_t m = p - 1;
        while (m > 1)
        {
            uint64_t q = smallest_prime_factor(m);
            uint64_t f = 0;
            while (m % q == 0)
            {
                m /= q;
                ++f;
            }
            add_or_update(q, f, lpr, lex, &lcnt);
        }
    }

    /* compute the product of prime powers */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < lcnt; ++i)
    {
        seq_t mult = seq_pow((seq_t)lpr[i], (idx_t)lex[i]);
        result = seq_mul(result, mult);
    }

    return result;
}
