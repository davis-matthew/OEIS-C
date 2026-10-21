/*
 * A230076  a(n) = (A007521(n)-1)/4.
 * https://oeis.org/A230076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p - 1)/4 where p is the nth prime congruent 5 mod 8
 * Range:     u8 n <= 14, u16 n <= 1808, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    static idx_t *primes = NULL;
    static size_t primes_cnt = 0;
    static size_t primes_cap = 0;

    if (n == 0) return 0;

    /* Generate enough primes of the form 8k+5 to satisfy request */
    while (primes_cnt < n)
    {
        idx_t candidate = (primes_cnt == 0) ? 5 : primes[primes_cnt - 1] + 8;

        /* Scan numbers 8 apart until we collect the required amount */
        while (primes_cnt < n)
        {
            if (isPrimeGeneric((seq_t)candidate))
            {
                if (primes_cnt == primes_cap)
                {
                    primes_cap = (primes_cap ? primes_cap << 1 : 16);
                    primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
                }
                primes[primes_cnt++] = candidate;
            }
            candidate += 8;
        }
    }

    seq_t p = (seq_t)primes[n - 1];
    return (p - 1) / 4;
}
