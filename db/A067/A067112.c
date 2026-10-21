/*
 * A067112  Let N = 2357111317192329313741434753..., the concatenation of the
 *          primes; a(n) = sum of first n digits of N.
 * https://oeis.org/A067112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = sum of the first n digits of
 *            2357111317192329313741434753… (i.e. the concatenation of all
 *            primes)
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function caches the cumulative sums and the list of primes,
   extending them as necessary to answer any requested index.   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state preserved between calls */
    static seq_t *cum = NULL;          /* cumulative sums: cum[0] = 0          */
    static size_t  cum_size = 0;        /* number of elements in cum         */
    static size_t  cum_cap  = 0;        /* allocated capacity of cum         */

    static uint64_t *primes = NULL;    /* list of primes found so far         */
    static size_t   prime_cnt = 0;      /* number of primes stored             */
    static size_t   prime_cap = 0;      /* allocated capacity of primes        */

    static uint64_t candidate = 2;      /* next integer to test for primality  */

    /* one‑time initialization */
    if (!cum) {
        cum_cap  = 128;
        cum      = (seq_t *)malloc(cum_cap * sizeof(seq_t));
        cum[0] = (seq_t)0;
        cum_size = 1;
    }
    if (!primes) {
        prime_cap = 128;
        primes    = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_cnt = 0;
    }

    /* If we already have the requested term, return it */
    if (n < cum_size)
        return cum[(size_t)n];

    /* Extend the cache until we know a(n) */
    while (cum_size <= n) {
        /* ---- primality test for 'candidate' ---- */
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* store the new prime */
            if (prime_cnt >= prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 128;
                primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
            }
            primes[prime_cnt++] = candidate;

            /* extract decimal digits (left‑to‑right) */
            uint64_t temp = candidate;
            unsigned int digits[20];
            int dlen = 0;
            do {
                digits[dlen++] = (unsigned int)(temp % 10);
                temp /= 10;
            } while (temp != 0);

            for (int i = dlen - 1; i >= 0; --i) {
                /* grow cumulative array if needed */
                if (cum_size >= cum_cap) {
                    cum_cap = cum_cap ? cum_cap * 2 : 128;
                    cum = (seq_t *)realloc(cum, cum_cap * sizeof(seq_t));
                }
                seq_t last = cum[cum_size - 1];
                cum[cum_size++] = last + (seq_t)digits[i];
            }
        }

        /* next candidate */
        ++candidate;
    }

    return cum[(size_t)n];
}
