/*
 * A218044  Numbers of the form 2^k + prime, with k > 0.
 * https://oeis.org/A218044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A218044:
   Numbers of the form 2^k + prime, with k > 0.
   a(n) is the n‑th smallest such number. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Initial search bound – roughly 4 times the index.
       It is doubled if insufficient terms are found. */
    idx_t bound = n * 4;
    if (bound < 64)
        bound = 64;          /* safety for very small n */

    for (;;)
    {
        /* ----------------------------------------------------------
           1.  Generate all primes up to 'bound' (Sieve of Eratosthenes)
        ---------------------------------------------------------- */
        bool *isprime = (bool *)malloc((size_t)(bound + 1));
        if (!isprime)
            return (seq_t)0;
        memset(isprime, true, (size_t)(bound + 1));
        if (bound >= 0) isprime[0] = false;
        if (bound >= 1) isprime[1] = false;

        for (idx_t i = 2; (idx_t)(i * i) <= bound; ++i)
        {
            if (isprime[i])
                for (idx_t j = i * i; j <= bound; j += i)
                    isprime[j] = false;
        }

        /* ----------------------------------------------------------
           2.  Mark all numbers that can be written as 2^k + prime
        ---------------------------------------------------------- */
        bool *present = (bool *)calloc((size_t)(bound + 1), 1);
        if (!present)
        {
            free(isprime);
            return (seq_t)0;
        }

        for (idx_t p = 2; p <= bound; ++p)
        {
            if (!isprime[p])
                continue;

            uint64_t pow2 = 2;  /* start with 2^1 */
            for (;;)
            {
                seq_t val = (seq_t)pow2 + (seq_t)p;
                if (val > (seq_t)bound)
                    break;
                present[(size_t)val] = true;
                pow2 <<= 1;
                if (pow2 > (uint64_t)bound)
                    break;
            }
        }

        /* ----------------------------------------------------------
           3.  Find the n‑th term by scanning the 'present' array
        ---------------------------------------------------------- */
        size_t count = 0;
        for (idx_t i = 2; i <= bound; ++i)
        {
            if (present[i])
            {
                ++count;
                if (count == n)
                {
                    free(isprime);
                    free(present);
                    return (seq_t)i;
                }
            }
        }

        /* ----------------------------------------------------------
           4.  Not enough terms – enlarge the bound and try again
        ---------------------------------------------------------- */
        free(isprime);
        free(present);
        bound <<= 1;   /* double the search bound */
    }
}
