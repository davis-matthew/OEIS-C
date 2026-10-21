/*
 * A054270  Largest prime below prime(n)^2 (A001248).
 * https://oeis.org/A054270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime < prime(n)^2
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A054270: the largest prime
 * strictly less than the square of the n-th prime.
 * The function is memoised: the first n primes are generated once
 * and reused for subsequent calls.  For each query it simply tests
 * decreasing odd candidates until a prime is found.  The routine
 * remains efficient for n up to at least 10 000. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/is_prime.h"

static uint32_t *prime_cache = NULL;   /* cached primes as 32‑bit ints */
static size_t  prime_cache_cnt = 0;    /* number of primes stored */

/* Ensure that at least 'needed' primes are cached.
 * The cache is rebuilt when increasing the size. */
static void
ensure_primes(idx_t needed)
{
    if (prime_cache_cnt >= needed)
        return;
    if (prime_cache)
        free(prime_cache);

    /* build_first_n_primes allocates memory that must be freed. */
    sieve_first_n_primes(needed, &prime_cache, &prime_cache_cnt);
}

/* --------------------------------------------------------------- */
/* Main entry point required by the framework.                     */
seq_t
a(idx_t n)
{
    if (n == 0)   /* OEIS offset is 1; behavior below offset is irrelevant */
        return 0;

    /* Prepare the list of primes up to the n‑th one.  This is O(n log log n)
     * once per distinct n.  Re‑using the cache keeps later calls fast. */
    ensure_primes(n);

    /* Retrieve p_n (as 64‑bit to avoid overflow during squaring). */
    const uint64_t p_n = (uint64_t)prime_cache[n - 1];

    /* Compute the square as seq_t to accommodate arbitrary‑precision
     * builds.  For n up to 10000 the value fits comfortably in 64 bits. */
    const seq_t limit = (seq_t)p_n * (seq_t)p_n;

    /* Search downward for the largest prime < limit. */
    seq_t candidate = limit - 1;          /* start just below the limit   */

    /* Make sure candidate is odd (except for the trivial prime 2). */
    if (candidate > 2 && (candidate & 1) == 0)
        candidate--;

    /* Walk down by two (skipping even numbers), stopping when a prime is
     * found.  2 is the only even prime and will be hit only if all odd
     * candidates fail, which cannot happen for the defined domain. */
    while (!is_prime((idx_t)candidate)) {
        /* The sequence guarantees that a prime will be found.
         * For safety we break at 2 (the smallest prime). */
        if (candidate <= 3) {
            candidate = 2;
            break;
        }
        candidate -= 2;
    }

    return (seq_t)candidate;
}
