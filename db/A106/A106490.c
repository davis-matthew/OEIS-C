/*
 * A106490  Total number of bases and exponents in Quetian Superfactorization
 *          of n, excluding the unity-exponents at the tips of branches.
 * https://oeis.org/A106490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p^e || n} (1 + a(e)), a(1) = 0 (i.e., for each prime
 *            factor p^e of n, add 1 for the base p plus the value of a
 *            applied to the exponent e)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Base case: n = 1 contributes nothing. */
    if (n <= 1) return 0;

    seq_t result = 0;
    idx_t x = n;

    /* Trial division – factor x into prime powers. */
    for (idx_t p = 2; p <= x / p; ++p)
    {
        if (x % p == 0)
        {
            idx_t e = 0;
            while (x % p == 0)
            {
                x /= p;
                ++e;
            }
            /* For the prime factor p^e, add 1 + a(e). */
            result += (seq_t)1 + a(e);
        }
    }
    /* If x > 1, we have a remaining prime factor with exponent 1. */
    if (x > 1)
    {
        /* 1 + a(1) == 1. */
        result += (seq_t)1;
    }

    return result;
}
