/*
 * A166662  Totally multiplicative sequence with a(p) = 4p+1 for prime p.
 * https://oeis.org/A166662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime factors p^e of (4p+1)^e
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 1535, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, return 0 */
    seq_t result = 1;

    /* factor n */
    uint64_t temp = (uint64_t)n;

    /* factor 2 */
    if ((temp & 1ULL) == 0)
    {
        int e = 0;
        while ((temp & 1ULL) == 0)
        {
            temp >>= 1;
            ++e;
        }
        seq_t base = (seq_t)4 * (seq_t)2 + 1;          /* 4*2+1 = 9 */
        seq_t factor = seq_pow(base, (idx_t)e);
        result *= factor;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            seq_t base = (seq_t)4 * (seq_t)p + 1;
            seq_t factor = seq_pow(base, (idx_t)e);
            result *= factor;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1)
    {
        seq_t base = (seq_t)4 * (seq_t)temp + 1;
        result *= base;   /* exponent is 1 */
    }

    return result;
}
