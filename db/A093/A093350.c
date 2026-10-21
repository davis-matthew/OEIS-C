/*
 * A093350  Primes congruent to 6 mod 13.
 * https://oeis.org/A093350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p ≡ 19 (mod 26) (offset 1)
 * Range:     u8 n <= 5, u16 n <= 505, u32 n <= 520, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/seq_realloc.h"
#include "../utils/is_prime_big.h"

static seq_t *prime_cache = NULL;   /* cache of generated primes */
static size_t prime_count = 0;      /* number stored in prime_cache   */
static size_t prime_cap   = 0;      /* allocated capacity */

/* Ensure that at least `needed` primes have been generated. */
static void ensure_prime_cache(idx_t needed)
{
    if (needed <= (idx_t)prime_count) return;

    /* Expand the cache if necessary. */
    if ((size_t)needed > prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        if (new_cap < (size_t)needed) new_cap = needed;
        prime_cache = seq_realloc(prime_cache, prime_cap, new_cap);
        prime_cap   = new_cap;
    }

    /* Generate the missing primes. */
    for (; prime_count < (size_t)needed; ++prime_count) {
        seq_t candidate;
        if (prime_count == 0) {
            candidate = (seq_t)19;
        } else {
            /* Start at the next number in the residue class 19 mod 26. */
            candidate = prime_cache[prime_count - 1] + (seq_t)26;
        }
        /* Search until a prime is found. */
        while (!is_prime_big(candidate))
            candidate = candidate + (seq_t)26;
        prime_cache[prime_count] = candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for indices below the offset */
    ensure_prime_cache(n);
    return prime_cache[n - 1];
}
