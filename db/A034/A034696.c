/*
 * A034696  Dirichlet convolution of primes (A000040) with themselves.
 * https://oeis.org/A034696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} prime(d) * prime(n/d) where prime(d) is the
 *            d‑th prime number (prime(1)=2, prime(2)=3, …)
 * Range:     u8 n <= 11, u16 n <= 335, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Store the computed prime numbers (numeric values), 1‑based indexing
   is presented as primes[0] = 2, primes[1] = 3, … . */
static uint64_t *primes = NULL;   /* array of primes up to the current limit */
static size_t   prime_count = 0;   /* number of primes stored */

/* Ensure that the array contains at least the first n primes.
   For n <= 10000 the bound 20*n+100 is safely larger than the n‑th prime. */
static void
ensure_primes(idx_t n)
{
    if (n <= prime_count)
        return;                       /* already enough primes */

    /* Compute a numeric upper bound large enough to include the n‑th prime. */
    idx_t limit = (idx_t)(20ULL * n) + 100ULL;
    if (limit < 1000ULL)
        limit = 1000ULL;               /* guard for very small n */

    uint64_t *new_primes = NULL;
    size_t   new_cnt    = 0;

    /* build_primes_upto generates all primes <= limit. */
    build_primes_upto(limit, &new_primes, &new_cnt);

    /* Replace old array (if any) with the new one. */
    if (primes)
        free(primes);
    primes      = new_primes;
    prime_count = new_cnt;
}

/* Dirichlet convolution of the prime sequence with itself. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n < 1, safety only */

    ensure_primes(n);                   /* guarantees primes[0..n-1] exist */

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;
            seq_t pd  = (seq_t)primes[d - 1];
            seq_t pn2 = (seq_t)primes[d2 - 1];
            seq_t prod = pd * pn2;
            sum += prod;
            if (d != d2)                 /* account for the symmetric divisor */
                sum += prod;
        }
    }
    return sum;
}
