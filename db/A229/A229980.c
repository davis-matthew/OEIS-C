/*
 * A229980  Smallest integer m > 0 such that p(n) + m*p(n + 1) and m*p(n) +
 *          p(n + 1) are prime, p(n) = n-th prime.
 * https://oeis.org/A229980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m > 0 such that p(n)+m*p(n+1) and m*p(n)+p(n+1)
 *            are prime
 * Range:     u8 n <= 6, u16 n <= 97, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/isPrimeGeneric.h"

static uint64_t *prime_list = NULL;   /* cache of primes */
static size_t prime_count = 0;        /* number of cached primes */

/* Ensure that the cache contains at least `need` primes. */
static void ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;                       /* already have enough */

    /* Get an upper bound for the `need`‑th prime. */
    idx_t limit = _prime_limit_for_idx((idx_t)need);

    /* Rebuild the cache. */
    free(prime_list);
    prime_list = NULL;
    prime_count = 0;
    build_primes_upto(limit, &prime_list, &prime_count);
}

seq_t a(idx_t n)
{
    /* n is 1‑based; undefined for n = 0 */
    if (n == 0)
        return 0;

    /* Need primes p(n) and p(n+1). */
    ensure_primes((size_t)(n + 1));

    uint64_t p_n    = prime_list[(size_t)(n - 1)];
    uint64_t p_next = prime_list[(size_t)(n)];   /* p(n+1) */

    for (idx_t m = 1; ; ++m)
    {
        seq_t val1 = (seq_t)p_n + (seq_t)m * (seq_t)p_next;
        seq_t val2 = (seq_t)m * (seq_t)p_n + (seq_t)p_next;

        if (isPrimeGeneric(val1) && isPrimeGeneric(val2))
            return (seq_t)m;
    }
}
