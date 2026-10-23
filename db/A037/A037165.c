/*
 * A037165  a(n) = prime(n)*prime(n+1) - prime(n) - prime(n+1).
 * https://oeis.org/A037165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)*prime(n+1) - prime(n) - prime(n+1)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/* Static cache for the generated primes.  The array holds all primes
 * up to some limit large enough to cover at least the requested
 * index.  The cache is rebuilt only when a larger index is asked. */
static uint64_t *prime_table = NULL;   /* array of primes (as uint64_t) */
static size_t   prime_count = 0;       /* number of primes currently cached */

/* Ensure that at least indices 0 … n are available in prime_table. */
static void ensure_primes_upto(idx_t n)
{
    /* Already enough primes? */
    if (prime_table != NULL && n < prime_count)
        return;

    /* Compute a sieve limit that guarantees at least n+1 primes. */
    idx_t limit = _prime_limit_for_idx(n + 1);
    if (limit == 0)          /* safety for the special case n == 0 */
        limit = 2;

    /* Allocate a new prime list up to this limit. */
    uint64_t *new_primes = NULL;
    size_t    new_count  = 0;
    build_primes_upto(limit, &new_primes, &new_count);

    /* Replace the old table. */
    free(prime_table);
    prime_table = new_primes;
    prime_count = new_count;
}

/* Public function: compute the n‑th term of sequence A037165. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1 */

    /* Make sure we have the (n)th and (n+1)th primes. */
    ensure_primes_upto(n);

    size_t idx1 = (size_t)(n - 1);    /* prime(n) */
    size_t idx2 = (size_t)n;          /* prime(n+1) */

    seq_t p = (seq_t)prime_table[idx1];
    seq_t q = (seq_t)prime_table[idx2];

    return p * q - p - q;
}
