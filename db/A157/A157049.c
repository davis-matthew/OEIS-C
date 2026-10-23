/*
 * A157049  a(n) = the largest integer < p(n+1) that is coprime to
 *          (p(n+1)-p(n)), where p(n) is the n-th prime.
 * https://oeis.org/A157049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest integer < p(n+1) that is coprime to
 *            (p(n+1)-p(n)), where p(k) is the k‑th prime
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/_prime_limit_for_idx.h"   /* idx_t _prime_limit_for_idx(idx_t n); */
#include "../utils/build_primes_upto.h"      /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */
#include "../utils/gcd.h"                    /* seq_t gcd(seq_t a, seq_t b); */

/* Prime cache: array of primes up to the currently known upper bound.  */
static uint64_t *prime_cache = NULL;
static size_t   prime_cache_len = 0;

/* Ensure that prime_cache contains at least need_idx primes
   (i.e., the primes p(1)...p(need_idx)).  */
static void ensure_prime_cache(idx_t need_idx)
{
    if (prime_cache_len >= (size_t)need_idx)
        return;                          /* already sufficient */

    /* Free previous cache if any.  */
    if (prime_cache != NULL) {
        free(prime_cache);
        prime_cache = NULL;
        prime_cache_len = 0;
    }

    /* Compute an upper bound on the need_idx‑th prime.  */
    idx_t limit = _prime_limit_for_idx(need_idx);
    if (limit < 10)
        limit = 10;                       /* safety margin */

    /* Build the new cache.  */
    build_primes_upto(limit, &prime_cache, &prime_cache_len);
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0; use 0 as placeholder */
        return 0;

    /* We need primes up to the (n+1)-th prime.  */
    ensure_prime_cache(n + 1);

    /* p(n) is at index n‑1 (array is 0‑based).  */
    uint64_t p_n   = prime_cache[n - 1];
    uint64_t p_np1 = prime_cache[n];

    seq_t diff      = (seq_t)(p_np1 - p_n);         /* p(n+1)-p(n)  */
    seq_t candidate = (seq_t)(p_np1 - 1);           /* start just below p(n+1) */

    /* Move downward until we find a coprime to the prime gap.  */
    while (gcd(candidate, diff) != (seq_t)1)
        candidate--;

    return candidate;
}
