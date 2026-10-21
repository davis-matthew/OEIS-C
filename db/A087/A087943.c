/*
 * A087943  Numbers n such that 3 divides sigma(n).
 * https://oeis.org/A087943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 178, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate increasing integers and test whether
   σ(n) is divisible by 3 by checking prime factor exponents:
   a number n satisfies the condition iff it contains a prime
   p ≡ 1 (mod 3) with exponent ≡ 2 (mod 3) or a prime
   p ≡ 2 (mod 3) with an odd exponent. */

#include <stdint.h>
#include <stdbool.h>

/* Checks whether n has a prime factor satisfying the
   required exponent congruence. */
static bool good_number(uint64_t n)
{
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            int e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            if ((p % 3 == 1 && e % 3 == 2) ||
                (p % 3 == 2 && e % 2 == 1))
                return true;
        }
    }
    /* Remaining factor is > sqrt(n), hence prime. */
    if (m > 1)
    {
        if ((m % 3 == 1 && 1 % 3 == 2) || /* 1 % 3 == 2 is false */
            (m % 3 == 2 && 1 % 2 == 1))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t cur = 2;      /* sequence starts with 2 */
    while (1)
    {
        if (good_number(cur))
        {
            ++count;
            if (count == n)
                return (seq_t)cur;
        }
        ++cur;
    }
}
