/*
 * A079142  Numbers divisible by prime integer parts of their square roots.
 * https://oeis.org/A079142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(floor(n/3+1)) * ( prime(floor(n/3+1)) + (n mod 3) )
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static helper: compute the k-th prime (1‑based). */
static uint32_t kth_prime(idx_t k)
{
    /* primes are kept in a dynamically resizing array */
    static uint32_t *primes = NULL;   /* array of primes (1‑based)       */
    static size_t   prime_cnt = 0;    /* number of stored primes         */
    static size_t   prime_cap = 0;    /* allocated capacity              */
    static uint32_t last_checked = 1; /* last number tested for primality */

    if (k == 0) return 0;            /* should not happen */

    /* If we already have enough primes, return the stored one. */
    if ((size_t)k <= prime_cnt)
        return primes[k - 1];

    /* Otherwise extend the list until we have at least k primes. */
    for (uint32_t n = last_checked + 1; prime_cnt < (size_t)k; ++n)
    {
        /* check if n is prime using previously found primes */
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > n) break;   /* no divisor needed */
            if (n % p == 0)
            {
                is_prime = 0;
                break;
            }
        }

        if (is_prime)
        {
            /* grow array if needed */
            if (prime_cnt == prime_cap)
            {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                if (!tmp) abort();              /* memory exhausted */
                primes = tmp;
                prime_cap = new_cap;
            }
            primes[prime_cnt++] = n;
        }

        last_checked = n;
    }

    return primes[(size_t)k - 1];
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* compute k = floor(n/3) + 1 */
    idx_t k = n / 3 + 1;

    /* get the k-th prime */
    uint32_t p_uint = kth_prime(k);
    seq_t p = (seq_t)p_uint;

    /* r = n mod 3 */
    uint32_t r_uint = (uint32_t)(n % 3);
    seq_t r = (seq_t)r_uint;

    return p * (p + r);
}
