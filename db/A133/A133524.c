/*
 * A133524  Sum of squares of four consecutive primes.
 * https://oeis.org/A133524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n^2 + p_{n+1}^2 + p_{n+2}^2 + p_{n+3}^2, where p_k is
 *            the k‑th prime (OEIS A000040)
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 3511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"   /* provides sieve_first_n_primes */

/* ------------------------------------------------------------------ */
/* Cached list of the first N primes.  The cache grows only when an
 * larger N is requested.  prime_cache[i] holds the (i+1)-st prime.
 */
static uint32_t *prime_cache = NULL;
static size_t prime_cache_count = 0;

/* Ensure that the cache contains at least 'needed' primes.  If it does not,
 * free any existing buffer and re‑allocate a fresh one containing the
 * requested number of primes.
 */
static void ensure_primes(idx_t needed)
{
    if (prime_cache_count >= (size_t)needed) {
        return;
    }

    /* release previous allocation if any */
    if (prime_cache) {
        free(prime_cache);
        prime_cache = NULL;
    }

    /* sieve_first_n_primes allocates its own array; we just keep the
     * pointer and the count returned by the helper.
     */
    sieve_first_n_primes((idx_t)needed, &prime_cache, &prime_cache_count);
}

/* ------------------------------------------------------------------ */
/* Compute the OEIS sequence A133524: sum of the squares of four
 * consecutive primes, starting with the n-th prime (n >= 1).
 */
seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;           /* sequence is defined for n >= 1 */
    }

    /* We need primes p_n, p_{n+1}, p_{n+2}, p_{n+3}.  Thus we must have
     * at least n+3 primes in the cache.
     */
    idx_t required = n + 3;
    ensure_primes(required);

    seq_t sum = 0;
    /* prime_cache is 0‑based: prime_cache[0] = p_1.  So p_k = prime_cache[k-1]. */
    for (idx_t i = 0; i < 4; ++i) {
        uint32_t p = prime_cache[(size_t)(n - 1 + i)];
        seq_t sq = (seq_t)p * (seq_t)p;
        sum += sq;
    }
    return sum;
}
