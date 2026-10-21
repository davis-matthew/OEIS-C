/*
 * A200746  Completely multiplicative function with a(prime(k)) =
 *          prime(k)*prime(k-1), a(2) = 2.
 * https://oeis.org/A200746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, look
 * Formula:   Completely multiplicative function with a(p_k)=p_k*p_{k-1},
 *            a(2)=2. Compute a(n) = Π (a(p)^e) over prime factorization of n
 * Range:     u8 n <= 18, u16 n <= 262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n == 1) return 1;

    /* Static state for prime sieving and previous prime lookup */
    static uint64_t *primes = NULL;      /* array of primes */
    static size_t primes_cnt = 0;        /* number of primes stored */
    static size_t primes_limit = 0;      /* maximum value sieved up to */
    static uint64_t *prev_of = NULL;     /* prev_of[p] = previous prime of p (1 for 2) */
    static size_t prev_limit = 0;        /* maximum value stored in prev_of */

    uint64_t nn = (uint64_t)n;
    size_t needed_limit = nn >= 2 ? nn : 2;  /* we need primes up to n */

    /* Recompute primes and prev_of if necessary */
    if (needed_limit > primes_limit)
    {
        /* Free old data */
        free(primes);
        free(prev_of);

        /* Simple Eratosthenes sieve up to needed_limit */
        size_t limit = needed_limit;
        size_t sieve_size = limit + 1;
        uint8_t *is_comp = (uint8_t*)malloc(sieve_size);
        if (!is_comp) return 0; /* allocation failure, return 0 as dummy */
        memset(is_comp, 0, sieve_size);

        /* Allocate enough space for primes (limit+1 is safely big) */
        primes = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
        if (!primes) { free(is_comp); return 0; }
        primes_cnt = 0;

        for (size_t i = 2; i <= limit; ++i)
        {
            if (!is_comp[i])
            {
                primes[primes_cnt++] = i;
                if (i * i <= limit)
                {
                    for (size_t j = i * i; j <= limit; j += i)
                        is_comp[j] = 1;
                }
            }
        }
        free(is_comp);
        primes_limit = limit;

        /* Build previous‑prime table */
        prev_of = (uint64_t*)calloc(limit + 1, sizeof(uint64_t));
        if (!prev_of) return 0; /* allocation failure */
        uint64_t prev = 1;  /* define previous prime of 2 as 1 */
        for (size_t i = 0; i < primes_cnt; ++i)
        {
            uint64_t p = primes[i];
            prev_of[p] = prev;
            prev = p;
        }
        prev_limit = limit;
    }

    /* Factor n and accumulate product */
    seq_t result = 1;
    uint64_t temp = nn;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i)
    {
        uint64_t p = primes[i];
        int count = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++count;
        }
        if (count > 0)
        {
            seq_t a_p = (seq_t)p * (seq_t)prev_of[p];
            for (int k = 0; k < count; ++k)
                result *= a_p;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1)
    {
        seq_t a_p = (seq_t)temp * (seq_t)prev_of[temp];
        result *= a_p;
    }

    return result;
}
