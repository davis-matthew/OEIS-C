/*
 * A033196  a(n) = n^3 * Product_{p|n, p prime} (1 + 1/p).
 * https://oeis.org/A033196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^3 * Product_{p|n, p prime} (1 + 1/p)
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A direct implementation using prime factorisation of n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* invalid, but defined safely */

    seq_t result = (seq_t)n * (seq_t)n * (seq_t)n;   /* n^3 */
    uint64_t temp   = (uint64_t)n;                     /* copy for factoring */

    for (uint64_t p = 2; p <= temp / p; ++p)          /* trial division */
    {
        if (temp % p == 0)           /* p is a distinct prime factor */
        {
            result = (result / (seq_t)p) * ((seq_t)p + 1);

            while (temp % p == 0)   /* remove all powers of p */
                temp /= p;
        }
    }

    if (temp > 1)                     /* remaining prime larger than sqrt(n) */
    {
        uint64_t p = temp;
        result = (result / (seq_t)p) * ((seq_t)p + 1);
    }

    return result;
}
