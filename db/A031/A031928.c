/*
 * A031928  Lower prime of a difference of 10 between consecutive primes.
 * https://oeis.org/A031928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lower prime of consecutive primes whose difference equals
 *            10
 * Range:     u8 n <= 3, u16 n <= 625, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage shared between calls */
static uint64_t *primes     = NULL; /* array of primes up to prime_limit */
static size_t   prime_cnt   = 0;    /* number of primes stored */
static size_t   prime_limit = 0;    /* current sieved upper bound */
static size_t   gaps        = 0;    /* number of gaps equal to 10 */

/* ------------------------------------------------------------------ */
/* Rebuild the sieve up to `new_limit`.  All data is recomputed from
   scratch and stored in the static variables `primes`, `prime_cnt`,
   `prime_limit` and `gaps`.                                           */
/* ------------------------------------------------------------------ */
static void rebuild_sieve(idx_t new_limit)
{
    /* Allocate sieve array */
    bool *sieve = (bool *)malloc(sizeof(bool) * (size_t)(new_limit + 1));
    if (!sieve)
        exit(EXIT_FAILURE);

    /* initialise */
    for (size_t i = 0; i <= (size_t)new_limit; ++i)
        sieve[i] = true;
    if (new_limit >= 0) sieve[0] = false;
    if (new_limit >= 1) sieve[1] = false;

    /* Eratosthenes */
    for (size_t i = 2; (uint64_t)i * (uint64_t)i <= (uint64_t)new_limit; ++i) {
        if (sieve[i]) {
            for (uint64_t j = (uint64_t)i * (uint64_t)i;
                 j <= (uint64_t)new_limit; j += i)
                sieve[(size_t)j] = false;
        }
    }

    /* count primes */
    size_t count = 0;
    for (size_t i = 2; i <= (size_t)new_limit; ++i)
        if (sieve[i]) ++count;

    /* reallocate primes array */
    uint64_t *new_primes = (uint64_t *)realloc(primes, sizeof(uint64_t) * count);
    if (!new_primes) {
        free(sieve);
        exit(EXIT_FAILURE);
    }
    primes = new_primes;
    prime_cnt = count;

    /* fill primes array and count 10‑gaps */
    size_t idx = 0;
    uint64_t prev_prime = 0;
    gaps = 0;
    for (size_t i = 2; i <= (size_t)new_limit; ++i) {
        if (sieve[i]) {
            primes[idx++] = (uint64_t)i;
            if (i - prev_prime == 10)
                ++gaps;
            prev_prime = i;
        }
    }

    free(sieve);
    prime_limit = (size_t)new_limit;
}

/* ------------------------------------------------------------------ */
/* Ensure that at least `required` gaps of size 10 have been found.
   If not, grow the sieve until enough gaps are present.               */
/* ------------------------------------------------------------------ */
static void ensure_gaps(idx_t required)
{
    if (gaps >= (size_t)required)
        return;

    idx_t limit = (idx_t)((required < 10) ? 1000 : required * 10);
    for (;;) {
        rebuild_sieve(limit);

        if (gaps >= (size_t)required)
            break;

        limit = limit * 2 + 1000;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the problem.                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1 */
        return (seq_t)0;

    /* make sure we have enough gaps */
    ensure_gaps(n);

    /* locate the nth 10‑gap */
    size_t found = 0;
    uint64_t prev_prime = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (i > 0) {
            if (p - prev_prime == 10) {
                ++found;
                if (found == (size_t)n)
                    return (seq_t)(p - 10); /* lower prime of the gap */
            }
        }
        prev_prime = p;
    }

    /* should never reach here for valid n */
    return (seq_t)0;
}
