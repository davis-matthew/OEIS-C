/*
 * A177946  a(n) = prime(n)! / n!.
 * https://oeis.org/A177946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)! / n! = ∏_{k=n+1}^{prime(n)} k
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>           /* free, size_t */
#include "../utils/build_primes.h"  /* build_primes() */
#include "../utils/_prime_limit_for_idx.h" /* _prime_limit_for_idx() */

static idx_t *primes = NULL;     /* array of primes, 0‑based */
static size_t  prime_count = 0;  /* number of primes stored */

/* Ensure that primes[0..n-1] exist (i.e. at least n primes are available). */
static void
ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;                           /* already have enough */

    /* Free old storage if any */
    if (primes) {
        free(primes);
        primes = NULL;
        prime_count = 0;
    }

    /* Get an upper bound on the n‑th prime; this guarantees at least n primes. */
    idx_t limit = _prime_limit_for_idx(n);

    /* Build the list of primes up to the computed limit. */
    build_primes(limit, &primes, &prime_count);
}

/* -------------------------------------------------------------- */
/* Main sequence function: a(n) = prime(n)! / n!                  */
seq_t a(idx_t n)
{
    if (n == 0)          /* below the offset; behaviour undefined */
        return 0;

    /* Make sure we have the n‑th prime available. */
    ensure_primes(n);

    /* Retrieve the n‑th prime (array is 0‑based). */
    idx_t p = primes[n - 1];

    /* Compute the product ∏_{k=n+1}^{p} k. */
    seq_t result = 1;
    for (idx_t k = n + 1; k <= p; ++k)
    {
        result = result * (seq_t)k;
    }

    return result;
}
