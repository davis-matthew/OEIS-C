/*
 * A129363  Number of partitions of 2n into the sum of two twin primes.
 * https://oeis.org/A129363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search all twin primes ≤ 2n and count unordered pairs (p,q)
   with p + q = 2n where both p and q are twin primes. */

#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    idx_t N = 2 * n;                     /* target sum */
    static idx_t sieve_limit = 0;        /* maximum value sieved so far */
    static unsigned char *is_prime = NULL; /* primality array */

    /* Build or extend a sieve up to N */
    if (N > sieve_limit) {
        if (is_prime)
            free(is_prime);
        sieve_limit = N;
        is_prime = (unsigned char*)malloc((sieve_limit + 1) * sizeof(unsigned char));
        if (!is_prime)
            return (seq_t)0;          /* out‑of‑memory fallback */

        /* initialise all as prime */
        memset(is_prime, 1, sieve_limit + 1);
        if (sieve_limit >= 0) is_prime[0] = 0;
        if (sieve_limit >= 1) is_prime[1] = 0;

        /* Eratosthenes sieve */
        for (idx_t i = 2; i * i <= sieve_limit; ++i)
            if (is_prime[i])
                for (idx_t j = i * i; j <= sieve_limit; j += i)
                    is_prime[j] = 0;
    }

    seq_t count = 0;

    for (idx_t p = 2; p <= N / 2; ++p) {
        if (!is_prime[p])
            continue;

        /* Check whether p is a twin prime */
        int twin_p = 0;
        if (p >= 3) {
            if (p >= 2 + 2 && is_prime[p - 2])
                twin_p = 1;
            else if (p + 2 <= sieve_limit && is_prime[p + 2])
                twin_p = 1;
        }
        if (!twin_p)
            continue;

        idx_t q = N - p;
        if (q < p)   /* keep unordered pairs unique */
            continue;

        if (!is_prime[q])
            continue;

        /* Check whether q is a twin prime */
        int twin_q = 0;
        if (q >= 3) {
            if (q >= 2 + 2 && is_prime[q - 2])
                twin_q = 1;
            else if (q + 2 <= sieve_limit && is_prime[q + 2])
                twin_q = 1;
        }
        if (!twin_q)
            continue;

        ++count;
    }

    return count;
}
