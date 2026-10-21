/*
 * A232767  Primes p such that the concatenation p987654321 is prime.
 * https://oeis.org/A232767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   concat_parts, isPrimeGeneric, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: primes p such that the concatenation of p and 987654321 is prime.
 * The function builds the sequence on demand, storing found primes in a
 * static cache that is large enough for the expected range of indices
 * (up to about 10000).  The lookup works in any call order.
 */

#include <stddef.h>
#include <stdbool.h>

/* Helper functions provided by the build system. */
#include "../utils/is_prime.h"          /* bool is_prime(idx_t x); */
#include "../utils/isPrimeGeneric.h"    /* bool isPrimeGeneric(seq_t n); */
#include "../utils/concat_parts.h"      /* seq_t concat_parts(idx_t a, idx_t b); */

#define CACHE_CAP 20000      /* enough for n up to ~10000 */

/* Static cache for the sequence values. */
static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;
static idx_t next_candidate = 2;          /* next integer to test as a prime */

/* Main entry for the OEIS sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        /* Find the next prime candidate. */
        while (!is_prime(next_candidate))
            ++next_candidate;

        /* Build the concatenated number p 987654321. */
        seq_t cat = concat_parts(next_candidate, 987654321);

        /* Test primality of the concatenation. */
        if (isPrimeGeneric(cat))
        {
            if (cache_len >= CACHE_CAP)
                break; /* safety: cache is full */
            cache[cache_len++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }

    /* Return the requested term. */
    if (n <= cache_len)
        return cache[(size_t)n - 1];
    return 0;   /* fallback: should not happen for valid n */
}
