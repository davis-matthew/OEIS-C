/*
 * A158378  a(1) = 0, a(n) = gcd(A051904(n), A051903(n)) for n >= 2.
 * https://oeis.org/A158378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(minimum exponent in prime factorisation of n, maximum
 *            exponent in prime factorisation of n)
 * Range:     u8 n <= 9454, u16 n <= 9454, u32 n <= 9454, u64 n <= 9454,
 *            u128 n <= 9454, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with a(1)=0 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    /* cast index n to a native integer for factorisation */
    uint64_t num = (uint64_t)n;
    uint64_t temp = num;

    uint64_t minExp = 0, maxExp = 0;
    bool first = true;

    /* trial division up to sqrt(temp) */
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            if (first)
            {
                minExp = exp;
                maxExp = exp;
                first = false;
            }
            else
            {
                if (exp < minExp) minExp = exp;
                if (exp > maxExp) maxExp = exp;
            }
        }
    }

    /* any remaining prime factor contributes exponent 1 */
    if (temp > 1)
    {
        uint64_t exp = 1;
        if (first)
        {
            minExp = exp;
            maxExp = exp;
            first = false;
        }
        else
        {
            if (exp < minExp) minExp = exp;
            if (exp > maxExp) maxExp = exp;
        }
    }

    /* compute gcd of the two exponents */
    seq_t a_min = (seq_t)minExp;
    seq_t a_max = (seq_t)maxExp;
    seq_t g = gcd(a_min, a_max);
    return g;
}
