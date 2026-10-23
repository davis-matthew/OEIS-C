/*
 * A142366  Primes congruent to 15 mod 47.
 * https://oeis.org/A142366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 135, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes that are ≡ 15 (mod 47).  a(1)=109, a(2)=673, … */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* simple deterministic primality test for numbers up to ~10⁷ */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined below the offset */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static uint64_t candidate = 0;
    static bool initialized = false;

    if (!initialized) {
        /* first candidate ≡ 15 (mod 47) that is prime: 109 */
        candidate = 47ULL * 2ULL + 15ULL;
        initialized = true;
    }

    while (cache_size < (size_t)n) {
        /* search until we hit the next prime of the desired form */
        while (!is_prime_uint64(candidate))
            candidate += 47ULL;

        /* grow cache if necessary */
        if (cache_size == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_size++] = (seq_t)candidate;
        candidate += 47ULL;  /* advance to the next candidate */
    }

    return cache[n - 1];
}
