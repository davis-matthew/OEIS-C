/*
 * A134797  Odd isolated primes.
 * https://oeis.org/A134797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A134797: Odd isolated primes.
 * a(n) = the n-th odd prime p such that both p-2 and p+2 are composite.
 * (equivalent to A007510(n+1) with OEIS offset 1).
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* value for offset violation; harmless */

    /* Start with a reasonable bound and grow if needed. */
    idx_t limit = n * 20;
    if (limit < 30)
        limit = 30;   /* ensure we reach at least the first odd isolated prime */

    while (1)
    {
        size_t sieve_size = (size_t)limit + 3;   /* we need to test up to limit+2 for p+2 */
        uint8_t *isPrime = (uint8_t *)malloc(sieve_size);
        if (!isPrime)
            return 0;          /* allocation failure; caller will not expect this */

        memset(isPrime, 1, sieve_size);
        isPrime[0] = isPrime[1] = 0;

        /* Classic Eratosthenes sieve up to limit+2 */
        for (idx_t i = 2; (i * i) <= (limit + 2); ++i)
        {
            if (isPrime[i])
            {
                for (idx_t j = i * i; j <= limit + 2; j += i)
                    isPrime[j] = 0;
            }
        }

        idx_t counter = 0;
        seq_t answer = 0;

        /* Scan odd numbers for isolated primes */
        for (idx_t p = 3; p <= limit; p += 2)
        {
            if (isPrime[p] && !isPrime[p - 2] && !isPrime[p + 2])
            {
                ++counter;
                if (counter == n)
                {
                    answer = (seq_t)p;
                    free(isPrime);
                    return answer;
                }
            }
        }

        free(isPrime);
        /* Not enough primes found, double the limit and try again */
        limit <<= 1;
    }
}
