/*
 * A088967  Numbers n such that n+9 is a prime.
 * https://oeis.org/A088967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n+4) - 9
 * Range:     u8 n <= 52, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th positive integer n such that n+9 is prime, using primes. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

static uint64_t *primes = NULL;      /* array of primes */
static idx_t    primes_count = 0;    /* number of primes stored */

/* Ensure we have at least `needed_index` primes (1‑based).  Using a
   simple sieve of Eratosthenes.  The bound is over‑estimated to
   guarantee enough primes.  Each call may recompute the sieve for a
   larger bound, which is acceptable for n <= ~10000. */
static void
ensure_primes(idx_t needed_index)
{
    if (needed_index <= primes_count)
        return;

    /* Estimate upper bound for the needed prime using Rosser's theorem. */
    double k = (double)needed_index;
    uint64_t bound;
    if (needed_index < 6)
        bound = 15;
    else
        bound = (uint64_t)(k * (log(k) + log(log(k))) + 3 * k) + 10;

    /* Allocate and run sieve. */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(bound + 1));
    if (!is_prime) abort();
    memset(is_prime, 1, bound + 1);
    is_prime[0] = is_prime[1] = 0;
    for (uint64_t p = 2; p * p <= bound; ++p) {
        if (is_prime[p]) {
            for (uint64_t q = p * p; q <= bound; q += p)
                is_prime[q] = 0;
        }
    }

    /* Count primes and build array. */
    idx_t count = 0;
    for (uint64_t i = 2; i <= bound; ++i)
        if (is_prime[i]) ++count;

    uint64_t *new_primes = (uint64_t *)malloc((size_t)count * sizeof(uint64_t));
    if (!new_primes) abort();

    idx_t idx = 0;
    for (uint64_t i = 2; i <= bound; ++i)
        if (is_prime[i]) new_primes[idx++] = i;

    free(primes);
    free(is_prime);

    primes     = new_primes;
    primes_count = count;
}

seq_t
a(idx_t n)
{
    /* Map n to the (n+4)-th prime minus 9. */
    idx_t prime_index = n + 4;  /* 1‑based index of the prime */
    ensure_primes(prime_index);
    return (seq_t)(primes[prime_index - 1] - 9);
}
