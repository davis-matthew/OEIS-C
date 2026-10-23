/*
 * A135176  5^p + 3^p + 2^p, where p = prime(n).
 * https://oeis.org/A135176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^prime(n) + 3^prime(n) + 2^prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------
 * Prime generation utilities
 * ------------------------------------------------------------------ */

static idx_t      *primes      = NULL; /* array of primes (1‑based)      */
static size_t      prime_count = 0;    /* number of primes stored       */
static idx_t        prime_lim   = 0;    /* upper bound of the current sieve */

/* Ensure that at least `n` primes are available in `primes`.       */
static void
ensure_primes(idx_t n)
{
    /* Already enough primes? */
    if (prime_count >= (size_t)n)
        return;

    /* Initial limit if none exists. */
    if (prime_lim == 0)
        prime_lim = 100;
    if (prime_lim < 20)
        prime_lim = 20;

    /* Keep increasing limit until we have at least `n` primes.    */
    while (prime_count < (size_t)n) {
        /* Allocate a simple sieve array. */
        uint8_t *is_prime = (uint8_t *)malloc((size_t)(prime_lim + 1));
        if (!is_prime)
            return; /* allocation failure – give up */

        /* Initialise */
        for (idx_t i = 0; i <= prime_lim; ++i)
            is_prime[i] = 1;
        is_prime[0] = 0;
        is_prime[1] = 0;

        /* Sieve of Eratosthenes */
        for (idx_t i = 2; i * i <= prime_lim; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= prime_lim; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Count primes and build the array. */
        size_t cnt = 0;
        for (idx_t i = 2; i <= prime_lim; ++i)
            if (is_prime[i])
                ++cnt;

        if (cnt >= (size_t)n) {
            idx_t *new_primes = (idx_t *)malloc(cnt * sizeof(idx_t));
            if (!new_primes) {
                free(is_prime);
                return;
            }
            size_t idx = 0;
            for (idx_t i = 2; i <= prime_lim; ++i)
                if (is_prime[i])
                    new_primes[idx++] = i;

            /* Replace the old cache. */
            free(primes);
            primes = new_primes;
            prime_count = cnt;
            /* prime_lim stays the same; we already have enough primes. */
            free(is_prime);
            break;
        }

        /* Not enough primes – expand the limit and retry. */
        free(is_prime);
        prime_lim *= 2;
    }
}

/* ------------------------------------------------------------------
 * Integer exponentiation
 * ------------------------------------------------------------------ */

static seq_t
seq_pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b      = base;
    while (exp) {
        if (exp & 1U)
            result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

/* ------------------------------------------------------------------
 * Main sequence function
 * ------------------------------------------------------------------ */

seq_t
a(idx_t n)
{
    if (n < 1U)
        return (seq_t)0; /* undefined below offset */

    /* Make sure the nth prime is available. */
    ensure_primes(n);

    idx_t p = primes[n - 1]; /* primes array is zero‑based */

    seq_t pow5 = seq_pow_seq((seq_t)5, p);
    seq_t pow3 = seq_pow_seq((seq_t)3, p);
    seq_t pow2 = seq_pow_seq((seq_t)2, p);

    return pow5 + pow3 + pow2;
}
