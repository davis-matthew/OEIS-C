/*
 * A158955  First differences of A061241.
 * https://oeis.org/A158955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = prime_{n+1} - prime_{n} where prime_k are primes
 *            congruent to 7 modulo 9
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static state for primes and differences */
static uint64_t *primes     = NULL; /* primes ≡ 7 (mod 9) */
static size_t   primes_cap  = 0;
static size_t   primes_used = 0;

static seq_t *diffs   = NULL; /* a(n) values */
static size_t   diffs_cap  = 0;
static size_t   diffs_used = 0;

static uint64_t sieve_limit = 0;

/* Ensure we have at least 'required' primes (congruent to 7 mod 9). */
static void ensure_primes(size_t required)
{
    if (primes_used >= required)
        return;

    while (primes_used < required)
    {
        /* Choose a new limit for the sieve – grow geometrically. */
        uint64_t new_limit = sieve_limit ? sieve_limit * 2 : 200000ULL;

        /* Allocate boolean array for sieve. */
        bool *is_prime = (bool*)malloc((size_t)(new_limit + 1));
        if (!is_prime)
            return;   /* allocation failure – exit quietly */

        for (size_t i = 0; i <= new_limit; ++i)
            is_prime[i] = true;
        if (new_limit >= 0) { is_prime[0] = false; if (new_limit >= 1) is_prime[1] = false; }

        /* Classic Eratosthenes sieve. */
        for (uint64_t p = 2; p * p <= new_limit; ++p)
        {
            if (is_prime[p])
                for (uint64_t m = p * p; m <= new_limit; m += p)
                    is_prime[m] = false;
        }

        /* Extract primes ≡ 7 (mod 9) and compute differences. */
        for (uint64_t p = 2; p <= new_limit; ++p)
        {
            if (is_prime[p] && (p % 9ULL) == 7ULL)
            {
                if (primes_used == primes_cap)
                {
                    size_t new_cap = primes_cap ? primes_cap * 2 : 128;
                    uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
                    if (!tmp)
                    {
                        free(is_prime);
                        return;   /* allocation failure – exit quietly */
                    }
                    primes     = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_used++] = p;

                if (primes_used > 1)
                {
                    seq_t diff = (seq_t)(primes[primes_used - 1] - primes[primes_used - 2]);

                    if (diffs_used == diffs_cap)
                    {
                        size_t new_cap = diffs_cap ? diffs_cap * 2 : 128;
                        seq_t *tmp = (seq_t*)realloc(diffs, new_cap * sizeof(seq_t));
                        if (!tmp)
                        {
                            free(is_prime);
                            return;   /* allocation failure – exit quietly */
                        }
                        diffs     = tmp;
                        diffs_cap = new_cap;
                    }
                    diffs[diffs_used++] = diff;
                }
            }
        }
        free(is_prime);
        sieve_limit = new_limit;
    }
}

seq_t a(idx_t n)
{
    /* Offset 1 – a(1) is the first term listed. */
    if (n < 1ULL)
        return (seq_t)0;

    size_t idx = (size_t)n;
    if (idx > diffs_used)
        ensure_primes(idx + 1);   /* need idx+1 primes to produce idx differences */

    return diffs[idx - 1];
}
