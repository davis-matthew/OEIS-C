/*
 * A166638  Totally multiplicative sequence with a(p) = 8*(p-1) for prime p.
 * https://oeis.org/A166638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e of (8*(p-1))^e (OEIS
 *            A166638)
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute A166638 */
seq_t a(idx_t n)
{
    if (n <= 1)                  /* a(1) = 1, all indices < 1 are irrelevant  */
        return (seq_t)1;

    seq_t prod = (seq_t)1;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            unsigned int exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            /* (8*(p-1))^exp  */
            seq_t factor = (seq_t)8 * (seq_t)(p - 1);
            for (unsigned int i = 0; i < exp; ++i)
                prod *= factor;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1)
    {
        seq_t factor = (seq_t)8 * (seq_t)(m - 1);
        prod *= factor;
    }

    return prod;
}
