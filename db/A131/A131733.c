/*
 * A131733  Primes (A000040) - odds (A005408).
 * https://oeis.org/A131733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) - 2n + 1 where prime(n) is the n‑th prime
 *            (A000040)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Very small primality test for 64‐bit integers.
   Works well for primes up to ~1e6 and therefore for
   the 10 000th prime (104 729). */
static bool is_prime_uint64(uint64_t v)
{
    if (v < 2U)
        return false;
    if (v % 2U == 0U)
        return v == 2U;
    for (uint64_t d = 3U; d * d <= v; d += 2U)
        if (v % d == 0U)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0U)
        return 0;   /* undefined, just return 0 */

    /* --- Sieve of primes on demand ------------------------------------ */
    static uint64_t *prime_cache  = NULL;
    static size_t   cache_cap     = 0;
    static size_t   cache_cnt     = 0;
    static uint64_t next_candidate = 2U;

    /* Grow cache until we have at least n primes. */
    while (cache_cnt < (size_t)n)
    {
        if (cache_cnt == cache_cap)
        {
            cache_cap = (cache_cap == 0U) ? 16U : cache_cap * 2U;
            void *tmp = realloc(prime_cache, cache_cap * sizeof(uint64_t));
            prime_cache = (uint64_t*)tmp;
        }

        if (is_prime_uint64(next_candidate))
            prime_cache[cache_cnt++] = next_candidate;
        ++next_candidate;
    }

    /* nth prime (1‑based indexing) */
    uint64_t prime_val = prime_cache[(size_t)(n - 1U)];

    /* Compute the term in the target seq_t type. */
    seq_t term = (seq_t)prime_val - ((seq_t)n * (seq_t)2U) + (seq_t)1U;
    return term;
}
