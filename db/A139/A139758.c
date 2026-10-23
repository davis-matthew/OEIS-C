/*
 * A139758  a(n) is the smallest prime such that (a(n) - the n-th prime) is a
 *          power of 2.
 * https://oeis.org/A139758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(n) + 2^m, where m is the smallest integer such
 *            that the sum is prime
 * Range:     u8 n <= 17, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation retrieves the n‑th prime and increments it by successive
   powers of two until a prime is found.  The prime table is cached and
   expanded as needed using _prime_limit_for_idx to bound the search. */

#include <stdlib.h>
#include <limits.h>
#include "../utils/_prime_limit_for_idx.h"   /* _prime_limit_for_idx */
#include "../utils/build_primes.h"           /* build_primes */
#include "../utils/is_prime_big.h"           /* is_prime_big */

/* Cached prime list: 1‑based, prime_cache[0] is the first prime. */
static idx_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;

/* Return the n‑th prime (1‑based).  Builds new primes up to an
   upper bound for n if necessary. */
static idx_t nth_prime(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, not used */

    /* If already cached enough primes, return directly. */
    if (n <= prime_cache_cnt)
        return prime_cache[n - 1];

    /* Obtain an upper bound for the n‑th prime. */
    idx_t limit = _prime_limit_for_idx(n);

    /* Allocate a new prime array up to the requested limit. */
    idx_t *new_cache = NULL;
    size_t new_cnt = 0;
    build_primes(limit, &new_cache, &new_cnt);

    /* Replace old cache (free the old array). */
    free(prime_cache);
    prime_cache = new_cache;
    prime_cache_cnt = new_cnt;

    return prime_cache[n - 1];
}

/* Main term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* below offset, not defined */

    /* Obtain the n‑th prime. */
    idx_t p = nth_prime(n);

    /* Start with 2^0 = 1 and keep doubling until the sum is prime. */
    uint64_t delta = 1;           /* 2^k, starts at k=0 */
    while (1) {
        uint64_t cand = p + delta;
        /* Be defensive against overflow; the sequence only requires values
           up to 10000th term, well below 2^64, so this is just a guard. */
        if (cand < p) break;  /* overflow detected */

        /* Test primality; is_prime_big accepts seq_t, so cast. */
        if (is_prime_big((seq_t)cand))
            return (seq_t)cand;

        /* Prepare next power of two; stop if shift would overflow. */
        if (delta > (UINT64_MAX >> 1))
            break;  /* no further powers fit in uint64_t */

        delta <<= 1;
    }

    /* Should never reach here for valid n. Return 0 as a safe fallback. */
    return (seq_t)0;
}
