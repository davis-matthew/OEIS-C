/*
 * A014692  a(n) = prime(n) - (n-1).
 * https://oeis.org/A014692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) - (n-1)
 * Range:     u8 n <= 66, u16 n <= 7193, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes.h"

/* Cached prime table: primes[0] = 2, primes[1] = 3, ... */
static idx_t *primes = NULL;
static size_t prime_count = 0;

/* Ensure that primes[0..n-2] are available. */
static void ensure_primes(idx_t n)
{
    if (n <= prime_count)
        return;                 /* already have enough primes */

    /* Get a limit large enough to contain the n-th prime. */
    idx_t limit = _prime_limit_for_idx(n);

    /* Rebuild the table if necessary. */
    if (primes)
        free(primes);          /* free old array if any */

    build_primes(limit, &primes, &prime_count);
}

/* Sequence A014692: a(n) = prime(n) - (n-1),  n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; undefined for n<1 */
        return 0;

    ensure_primes(n);
    idx_t p_n = primes[n - 1];
    return (seq_t)p_n - (seq_t)(n - 1);
}
