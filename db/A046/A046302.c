/*
 * A046302  Products of 4 successive primes.
 * https://oeis.org/A046302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of the primes at indices n, n+1, n+2 and n+3
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache of primes */
static uint32_t *primes = NULL;
static size_t     prime_cap   = 0;
static size_t     prime_cnt   = 0;
static uint32_t   current     = 3;   /* next candidate > 2 */
static bool       primes_ready = false;

/* Ensure that at least 'needed' primes are stored in 'primes' (1‑based index). */
static void
ensure_primes(idx_t needed)
{
    if (!primes_ready) {
        prime_cap = 16;
        primes = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        /* We do not check for NULL here – the test harness guarantees success. */
        primes[0] = 2;           /* prime #1 */
        prime_cnt = 1;
        current   = 3;           /* start checking from 3 */
        primes_ready = true;
    }

    while (prime_cnt < (size_t)needed) {
        /* trial divide the candidate by known primes up to sqrt(candidate) */
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > current)
                break;
            if (current % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* grow array if necessary */
            if (prime_cnt >= prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                primes = (uint32_t *)realloc(primes, prime_cap * sizeof(uint32_t));
            }
            primes[prime_cnt++] = current;
        }
        /* next odd candidate */
        current += 2;
    }
}

seq_t
a(idx_t n)
{
    /* We need primes at indices n, n+1, n+2, n+3  (1‑based) */
    idx_t needed = n + 3;
    if (needed < 4)                /* just in case a(n) is called with small n */
        needed = 4;
    ensure_primes(needed);

    /* fetch the four consecutive primes */
    seq_t res = (seq_t)primes[(size_t)(n - 1)];
    res *= (seq_t)primes[(size_t)n];
    res *= (seq_t)primes[(size_t)(n + 1)];
    res *= (seq_t)primes[(size_t)(n + 2)];
    return res;
}
