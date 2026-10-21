/*
 * A095074  Primes in whose binary expansion the number of 0-bits is less than
 *          or equal to number of 1-bits.
 * https://oeis.org/A095074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 5007, u32 n <= 9858, u64 n <= 9858,
 *            u128 n <= 9858, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A095074: primes whose binary representation has
   zeros <= ones.  The function searches primes by sieving a range
   until the n-th qualifying prime is found.  It starts with a
   conservative bound and doubles it if necessary. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition.  Return 0 for the undefined n = 0 case. */
    if (n == 0)
        return 0;

    /* Rough initial bound: enough primes for n up to ~20 000. */
    idx_t limit = n * 20 + 100;

    for (;;)
    {
        /* Allocate a boolean array for the sieve. */
        bool *is_composite =
            (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
        if (!is_composite)
            return 0;               /* allocation failure */

        /* Initialise all entries to "prime". */
        for (size_t i = 0; i <= (size_t)limit; ++i)
            is_composite[i] = false;

        /* Standard Eratosthenes sieve. */
        for (idx_t p = 2; p * p <= limit; ++p)
        {
            if (!is_composite[p])
            {
                for (idx_t k = p * p; k <= limit; k += p)
                    is_composite[k] = true;
            }
        }

        /* Count qualifying primes. */
        idx_t seen = 0;
        for (idx_t x = 2; x <= limit; ++x)
        {
            if (!is_composite[x])
            {
                /* Count 1‑bits and total bits. */
                idx_t ones = 0, bits = 0, t = x;
                while (t != 0)
                {
                    ones += t & 1;
                    bits++;
                    t >>= 1;
                }
                if (bits - ones <= ones)   /* zeros ≤ ones */
                {
                    ++seen;
                    if (seen == n)
                    {
                        free(is_composite);
                        return (seq_t)x;
                    }
                }
            }
        }

        /* Not enough primes found; enlarge the sieve limit. */
        free(is_composite);
        limit *= 2;
    }
}
