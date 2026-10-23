/*
 * A127342  Product of 10 consecutive primes.
 * https://oeis.org/A127342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the 10 consecutive primes starting at the n-th
 *            prime (1‑based index), i.e. a(n) = prime(n) * prime(n+1) * ... *
 *            prime(n+9)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary utilities for prime generation and limits */
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/* Standard headers */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Cached list of primes and its size */
static uint64_t *primes = NULL;       /* array of primes */
static size_t   prime_count = 0;      /* number of primes in the array */
static idx_t    cached_limit = 0;     /* limit used to build the array */

/* Return the product of primes prime(n) … prime(n+9) */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so n >= 1.  For safety we clamp to 1. */
    if (n == 0)
        n = 1;

    /* Determine how many primes we need: up to the (n+9)-th prime. */
    idx_t required_primes = n + 9;

    /* Find an upper bound for the required prime index. */
    idx_t needed_limit = _prime_limit_for_idx(required_primes);

    /* Rebuild the prime table only if we need a larger limit. */
    if (primes == NULL || needed_limit > cached_limit) {
        free(primes);
        build_primes_upto(needed_limit, &primes, &prime_count);
        cached_limit = needed_limit;
    }

    /* Compute the product of the 10 consecutive primes. */
    seq_t prod = (seq_t)1;
    size_t start = (size_t)(n - 1); /* zero‑based index of the first prime */
    for (idx_t i = 0; i < 10; ++i) {
        prod *= (seq_t)primes[start + (size_t)i];
    }

    return prod;
}
