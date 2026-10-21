/*
 * A098903  Odd numbers whose number of distinct prime factors is also odd.
 * https://oeis.org/A098903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th odd integer whose number of distinct prime
 *            factors is odd (this is the definition of OEIS A098903). The
 *            program searches odd numbers in increasing order, counts
 *            distinct prime factors by trial division, and keeps those with
 *            an odd count
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    /* Static cache of already found terms. */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0; /* number of elements stored */
    static size_t cache_cap  = 0; /* allocated capacity */

    /* Next odd integer to test.  Start at 3 for the first term. */
    static idx_t next_candidate = 3;

    /* Generate terms until we have the n-th one. */
    while (cache_size < n)
    {
        /* Search for the next qualifying odd number. */
        for (;;)
        {
            /* We only need the value for factorisation; all tested
               numbers are odd, so 2 is never a factor. */
            uint64_t x    = (uint64_t)next_candidate;
            uint64_t tmp  = x;
            uint32_t distinct = 0;

            /* Trial division: try odd divisors up to sqrt(tmp). */
            for (uint64_t p = 3; p <= tmp / p; p += 2)
            {
                if ((tmp % p) == 0)
                {
                    ++distinct;
                    while ((tmp % p) == 0)
                        tmp /= p;
                }
            }
            /* If a prime factor remains, count it. */
            if (tmp > 1)
                ++distinct;

            /* Does this odd number have an odd number of distinct primes? */
            if ((distinct & 1) != 0)
            {
                /* Allocate more space if necessary. */
                if (cache_size == cache_cap)
                {
                    cache_cap = cache_cap ? cache_cap * 2 : 128;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                /* Store the term. */
                cache[cache_size++] = (seq_t)x;
                /* Advance to the next true odd candidate. */
                next_candidate += 2;
                break; /* done with this term */
            }

            /* Not accepted: try the next odd number. */
            next_candidate += 2;
        }
    }

    /* Return the requested term (1‑based). */
    return cache[n - 1];
}
