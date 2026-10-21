/*
 * A071414  Number of times 4n+r and 2n+r are both prime, with 0 < r < 2n.
 * https://oeis.org/A071414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number r such that 4n+r and 2n+r are both prime.  
   a(n) = |{ r : 1 ≤ r < 2n,  4n+r and 2n+r are prime }|  */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Local helper: integer square root (floor) for 64‑bit values. */
static idx_t
isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while (((idx_t)(r + 1) * (r + 1)) <= n) {
        ++r;
    }
    return r;
}

/* Sieve of Eratosthenes storage (static for memoisation). */
static uint8_t *prime_sieve = NULL;
static idx_t   sieve_limit = 0;

/* Ensure prime_sieve is filled for all numbers ≤ 6*n. */
static void
ensure_primes(idx_t n)
{
    uint64_t limit = 6ULL * (uint64_t)n;          /* maximum value needed   */
    if (prime_sieve && limit <= (uint64_t)sieve_limit) {
        return;                                  /* already sufficient   */
    }

    /* Rebuild the sieve. */
    free(prime_sieve);
    prime_sieve = (uint8_t *)malloc((size_t)(limit + 1));
    if (!prime_sieve) {
        abort();                                 /* allocation failure   */
    }

    /* Initialise all as prime, then clear 0 and 1. */
    memset(prime_sieve, 1, (size_t)(limit + 1));
    if (limit >= 0) prime_sieve[0] = 0;
    if (limit >= 1) prime_sieve[1] = 0;

    /* Eratosthenes elimination up to sqrt(limit). */
    idx_t max_root = isqrt_u64(limit);
    for (idx_t p = 2; p <= max_root; ++p) {
        if (!prime_sieve[p]) continue;
        uint64_t start = (uint64_t)p * (uint64_t)p;
        for (uint64_t m = start; m <= limit; m += (uint64_t)p) {
            prime_sieve[m] = 0;
        }
    }

    sieve_limit = limit;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, undefined for 0 */

    ensure_primes(n);              /* make sure we have primes up to 6n */

    seq_t count = 0;
    for (idx_t r = 1; r < 2 * n; ++r) {
        uint64_t val1 = 4ULL * (uint64_t)n + (uint64_t)r;
        uint64_t val2 = 2ULL * (uint64_t)n + (uint64_t)r;
        if (prime_sieve[val1] && prime_sieve[val2]) {
            ++count;
        }
    }
    return count;
}
