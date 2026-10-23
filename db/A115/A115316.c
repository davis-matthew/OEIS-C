/*
 * A115316  Lexicographically earliest permutation of the natural numbers such
 *          that each prime number is followed by exactly two composite
 *          numbers.
 * https://oeis.org/A115316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A115316: lexicographically earliest permutation of the natural
   numbers in which each prime is followed by exactly two composite numbers.
   The permutation has primes at indices congruent to 2 modulo 3; other indices
   are filled with composites in increasing order. The function builds the
   sequence lazily and caches the results for repeated calls. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cache for computed terms */
static seq_t *seq_cache    = NULL;
static size_t seq_cache_len  = 0;
static size_t seq_cache_cap  = 0;

/* State for prime generation */
static uint64_t prime_next_candidate = 2; /* next integer to test for primality */

/* State for composite generation */
static uint64_t composite_candidate = 1;  /* next integer to test for composite */

/* Simple primality test (sufficient for the range of values needed). */
static bool is_prime_simple(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

/* Returns the next unused prime number. */
static uint64_t next_prime(void)
{
    uint64_t p = prime_next_candidate;
    while (!is_prime_simple(p))
        ++p;
    prime_next_candidate = p + 1;
    return p;
}

/* Extends the cache to contain terms up to index n. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= seq_cache_len)
        return;

    size_t new_cap = seq_cache_cap == 0 ? 32 : seq_cache_cap;
    while (new_cap < (size_t)n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();
    seq_cache = new_cache;
    seq_cache_cap = new_cap;

    for (idx_t i = seq_cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i % 3 == 2) {          /* prime position */
            uint64_t p = next_prime();
            val = (seq_t)p;
        } else {                   /* composite position */
            while (is_prime_simple(composite_candidate))
                ++composite_candidate;
            val = (seq_t)composite_candidate;
            ++composite_candidate;
        }
        seq_cache[i - 1] = val;
    }
    seq_cache_len = (size_t)n;
}

/* Required function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    ensure_cache(n);
    return seq_cache[n - 1];
}
