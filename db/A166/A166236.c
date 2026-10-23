/*
 * A166236  Absolute value of (n-th odd prime minus n-th odd nonprime)/2.
 * https://oeis.org/A166236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = abs(A065091(n) - A014076(n)) / 2
 * Range:     u8 n <= 179, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A065091(n)  – n‑th odd prime (3,5,7,…) */
/*  A014076(n) – n‑th odd non‑prime (1,9,15,…)  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* External integer types supplied by the build */
typedef uint64_t idx_t;   /* index type */
/* seq_t is defined by the build */

static idx_t *odd_primes      = NULL;      /* n‑th odd prime   */
static size_t  odd_prime_cnt  = 0;
static idx_t *odd_nonprimes   = NULL;      /* n‑th odd non‑prime */
static size_t  odd_nonprime_cnt = 0;
static uint8_t *sieve         = NULL;      /* odd‑number sieve */
static idx_t  sieve_limit     = 0;

/* Ensure that we have at least n odd primes and n odd non‑primes */
static void _ensure_cache(idx_t n)
{
    if (odd_prime_cnt >= (size_t)n && odd_nonprime_cnt >= (size_t)n)
        return;                            /* already enough */

    idx_t limit = n < 10 ? 200 : n * 20;    /* initial guess */
    while (1) {
        /* Size of the odd sieve: each bit represents an odd number
           from 1 (index 0) up to limit. */
        size_t sieve_size = (limit >> 1) + 1;
        uint8_t *new_sieve = (uint8_t *)malloc(sieve_size);
        if (!new_sieve) abort();

        /* Assume all odd numbers >1 may be prime */
        for (size_t i = 0; i < sieve_size; ++i) new_sieve[i] = 1;
        new_sieve[0] = 0;                    /* 1 is not prime */

        /* Perform Sieve of Eratosthenes on odd numbers only */
        for (idx_t i = 1; (2 * i + 1) * (2 * i + 1) <= limit; ++i)
            if (new_sieve[i]) {
                idx_t p = 2 * i + 1;
                idx_t start = ((p * p) - 1) / 2;
                for (idx_t j = start; j < sieve_size; j += p)
                    new_sieve[j] = 0;
            }

        /* Count primes (excluding 1) and non‑primes (including 1) */
        size_t prime_cnt = 0;
        for (size_t i = 1; i < sieve_size; ++i)
            if (new_sieve[i]) ++prime_cnt;
        size_t nonprime_cnt = sieve_size - prime_cnt;   /* includes 1 */

        /* Check whether the current limit suffices */
        if (prime_cnt >= (size_t)n && nonprime_cnt >= (size_t)n) {
            /* Allocate arrays for primes and non‑primes */
            idx_t *tmp_primes    = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
            idx_t *tmp_nonprimes = (idx_t *)malloc(nonprime_cnt * sizeof(idx_t));
            if (!tmp_primes || !tmp_nonprimes) abort();

            /* Fill prime array */
            size_t pi = 0;
            for (size_t i = 1; i < sieve_size; ++i)
                if (new_sieve[i])
                    tmp_primes[pi++] = 2 * i + 1;

            /* Fill non‑prime array (first 1, then odd non‑primes) */
            size_t ni = 0;
            tmp_nonprimes[ni++] = 1;               /* 1 is the first odd non‑prime */
            for (size_t i = 1; i < sieve_size; ++i)
                if (!new_sieve[i])
                    tmp_nonprimes[ni++] = 2 * i + 1;

            /* Replace old cache */
            free(sieve);
            free(odd_primes);
            free(odd_nonprimes);
            sieve = new_sieve;
            odd_primes = tmp_primes;
            odd_nonprimes = tmp_nonprimes;
            odd_prime_cnt = prime_cnt;
            odd_nonprime_cnt = nonprime_cnt;
            sieve_limit = limit;
            return;
        }

        /* Not enough; enlarge the limit and retry */
        free(new_sieve);
        limit <<= 1;               /* double the limit */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    _ensure_cache(n);

    /* n is 1‑based; arrays indexed from 0 */
    idx_t p = odd_primes[n - 1];        /* n‑th odd prime   */
    idx_t q = odd_nonprimes[n - 1];     /* n‑th odd non‑prime */

    /* Compute absolute difference */
    idx_t diff = (p > q) ? (p - q) : (q - p);

    /* Return |p - q| / 2 as seq_t */
    return (seq_t)(diff / 2);
}
