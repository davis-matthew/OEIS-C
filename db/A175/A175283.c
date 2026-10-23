/*
 * A175283  Numbers k with the property that k and k^2 + 3k+9 are primes.
 * https://oeis.org/A175283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 17, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find all k such that k and k^2 + 3k + 9 are primes.  A(n) is the
 * n‑th such integer (1‑indexed).  The implementation keeps a static
 * cache of the terms found so far, extending it incrementally.
 * Primality is tested with is_prime_big, which works for arbitrary‑
 * precision seq_t values. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"          /* is_prime_big()          */
#include "../utils/ensure_capacity.h"      /* ensure_capacity()       */

/* Return the n-th term of the sequence.  n >= 1. */
seq_t a(idx_t n)
{
    /* Static cache of terms found so far. */
    static seq_t *cache   = NULL;   /* array of found k's   */
    static size_t cache_size = 0;   /* how many are stored  */
    static size_t cache_cap  = 0;   /* allocated capacity   */
    static idx_t  next_candidate = 1;/* next integer to test */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        /* Move to the next integer candidate. */
        next_candidate++;

        /* Convert to seq_t for arithmetic. */
        seq_t k = (seq_t)next_candidate;
        if (!is_prime_big(k))
            continue;                     /* candidate not prime    */

        /* Compute s = k^2 + 3k + 9 using seq_t arithmetic. */
        seq_t s = k * k + 3 * k + 9;
        if (!is_prime_big(s))
            continue;                     /* the quadratic not prime */

        /* Grow cache if necessary. */
        if (cache_size == cache_cap)
            ensure_capacity(cache_size + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));

        /* Store the found term. */
        cache[cache_size++] = k;
    }

    return cache[(size_t)n - 1];
}
