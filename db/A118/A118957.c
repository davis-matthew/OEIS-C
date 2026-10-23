/*
 * A118957  Numbers of the form 2^k + p, where p is a prime less than 2^k.
 * https://oeis.org/A118957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^k + p where p is a prime < 2^k (OEIS A118957)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, return 0 for safety */
        return (seq_t)0;

    /* static storage for the sequence values */
    static seq_t *vals      = NULL;
    static size_t vals_len  = 0;
    static size_t vals_cap  = 0;

    /* static storage for generated primes */
    static uint64_t *primes       = NULL;
    static size_t primes_len      = 0;
    static size_t primes_cap      = 0;
    static uint64_t primes_limit  = 2;   /* next number to test for primality */

    /* keep producing terms until we have at least n of them */
    static idx_t current_k = 0;   /* last processed exponent */

    while (vals_len < (size_t)n)
    {
        /* next exponent k */
        ++current_k;
        uint64_t pow2 = (uint64_t)1 << current_k;   /* 2^k */

        /* generate all primes < pow2 */
        if (pow2 > primes_limit)
        {
            for (uint64_t x = primes_limit; x < pow2; ++x)
            {
                int isprime = 1;
                if (x < 2)
                    isprime = 0;
                else if (x == 2)
                    isprime = 1;
                else if ((x & 1) == 0)
                    isprime = 0;
                else
                {
                    for (size_t j = 1; j < primes_len; ++j)   /* primes[0]==2 */
                    {
                        uint64_t p = primes[j];
                        if (p > x / p)   /* p*p > x */
                            break;
                        if (x % p == 0)
                        {
                            isprime = 0;
                            break;
                        }
                    }
                }

                if (isprime)
                {
                    if (primes_len == primes_cap)
                    {
                        size_t new_cap = primes_cap == 0 ? 16 : primes_cap << 1;
                        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                        if (!tmp) abort();
                        primes = tmp;
                        primes_cap = new_cap;
                    }
                    primes[primes_len++] = x;
                }
            }
            primes_limit = pow2;
        }

        /* append terms 2^k + p for all primes p < 2^k */
        for (size_t i = 0; i < primes_len && primes[i] < pow2; ++i)
        {
            seq_t val = (seq_t)pow2 + (seq_t)primes[i];

            if (vals_len == vals_cap)
            {
                size_t new_cap = vals_cap == 0 ? 64 : vals_cap << 1;
                seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                vals = tmp;
                vals_cap = new_cap;
            }
            vals[vals_len++] = val;
        }
    }

    return vals[n - 1];
}
