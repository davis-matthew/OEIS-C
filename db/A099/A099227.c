/*
 * A099227  Primes of the form m^k+k, with m and k > 1.
 * https://oeis.org/A099227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 21, u32 n <= 1294, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_t, ensure_capacity, isPrimeGeneric, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form m^k + k (m > 1, k > 1) – search
 * -------------------------------------------------------------
 * a(n) returns the n‑th prime of the form m^k + k, with offset 1.
 * The function builds a cache of all such primes up to a dynamically
 * increasing bound and keeps it sorted and unique.  The cache is
 * expanded lazily until it contains at least n elements, then the
 * requested element is returned.
 *
 * Utilities from the framework are used:
 *   - pow_uint(base,exp)     : big integer exponentiation of a
 *     uint64_t base to an idx_t exponent, returning a seq_t.
 *   - isPrimeGeneric(n)     : primality test for a seq_t.
 *   - ensure_capacity(...)  : grow a malloc'd buffer.
 *   - cmp_seq_t(...)        : comparison function for seq_t (used
 *     by qsort).  The seq_t type may be std‑int or arbitrary‑precision.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/pow_uint.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/ensure_capacity.h"
#include "../utils/cmp_seq_t.h"

/* Static cache of primes of the form m^k + k (sorted, unique). */
static seq_t *prime_cache = NULL;    /* array of seq_t values */
static size_t  prime_len   = 0;      /* number of stored primes     */
static size_t  prime_cap   = 0;      /* allocated capacity */
static seq_t   last_limit  = 0;      /* largest value for which we
                                         have already generated all
                                         applicable primes           */

/* Helper: generate all primes of the form m^k + k that are > last_limit
 * and <= limit, appending them to the cache.  After inserting the new
 * primes, the whole cache is sorted and deduplicated. */
static void generate_primes_upto(seq_t limit)
{
    if (limit <= last_limit)
        return; /* nothing new to add */

    /* Produce all candidates and append those that are new and prime. */
    for (idx_t k = 2; ; ++k) {
        seq_t min_val = pow_uint((uint64_t)2, k) + k; /* m=2 gives smallest value for this k */
        if (min_val > limit)
            break; /* no further k will produce values <= limit */

        for (idx_t m = 2; ; ++m) {
            seq_t val = pow_uint((uint64_t)m, k) + k;
            if (val > limit)
                break;          /* this m (and larger m) is too big for this k */

            if (val > last_limit && isPrimeGeneric(val)) {
                /* Append val to cache, growing capacity as necessary. */
                ensure_capacity(prime_len + 1, &prime_cap,
                                (void **)&prime_cache, sizeof(seq_t));
                prime_cache[prime_len++] = val;
            }
        }
    }

    /* Sort the entire cache (existing + new entries). */
    if (prime_len > 1)
        qsort(prime_cache, prime_len, sizeof(seq_t), cmp_seq_t);

    /* Deduplicate: compact the sorted array in place. */
    if (prime_len > 1) {
        size_t uniq = 1;
        for (size_t i = 1; i < prime_len; ++i) {
            if (prime_cache[i] != prime_cache[uniq - 1]) {
                prime_cache[uniq++] = prime_cache[i];
            }
        }
        prime_len = uniq;
    }

    last_limit = limit;
}

/* Return the n-th prime of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n elements.  Expand
     * geometrically until we have enough. */
    while (prime_len < n) {
        seq_t new_limit = (last_limit == 0) ? 1000ULL
                                            : last_limit * 2ULL + 1000ULL;
        generate_primes_upto(new_limit);
    }
    return prime_cache[n - 1];
}
