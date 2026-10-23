/*
 * A152455  a(n) = minimal integer m such that there exists an m X m integer
 *          matrix of order n.
 * https://oeis.org/A152455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula/definition for OEIS A152455:
   a(1) = 0, a(2) = 1.
   If n mod 4 = 2 then a(n) = a(n/2).
   Otherwise a(n) = Σ_i (p_i - 1) * p_i^(e_i - 1),
   where n = Π_i p_i^e_i is the prime factorisation of n. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;
    if (n == 2)
        return 1;

    /* Recursive shortcut for n ≡ 2 (mod 4) */
    if ((n & 3) == 2)
        return a(n >> 1);   /* n/2 */

    seq_t result = 0;
    idx_t temp = n;
    int exp;

    /* Handle factor 2 separately */
    if ((temp & 1) == 0)
    {
        exp = 0;
        while ((temp & 1) == 0)
        {
            temp >>= 1;
            exp++;
        }
        seq_t term = 1;             /* 2^(exp-1) */
        for (int i = 0; i < exp - 1; ++i)
            term *= (seq_t)2;
        term *= (seq_t)(2 - 1);
        result += term;
    }

    /* Trial division by odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                exp++;
            }
            seq_t term = 1;             /* p^(exp-1) */
            for (int i = 0; i < exp - 1; ++i)
                term *= (seq_t)p;
            term *= (seq_t)(p - 1);
            result += term;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1)
    {
        seq_t term = 1;                 /* exp = 1 → p^(0) = 1 */
        term *= (seq_t)(temp - 1);
        result += term;
    }

    return result;
}
