/*
 * A036960  Primes containing only digits from the set (0,1,2,3,4,5,6).
 * https://oeis.org/A036960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose decimal digits belong to {0,1,2,3,4,5,6}.
   The function returns the n‑th such prime (offset 1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Test whether all decimal digits of v are ≤6. */
static bool allowed_digits(seq_t v)
{
    if (v <= 1) return false;
    while (v > 0) {
        int d = (int)(v % 10);
        if (d > 6) return false;
        v /= 10;
    }
    return true;
}

/* Static state for memoisation. */
static seq_t *cache       = NULL;
static size_t cache_cap   = 0;
static size_t cache_len   = 0;
static idx_t next_candidate = 1;   /* will be incremented to 2 on first call */

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* Keep generating primes until we have at least n terms. */
    while (cache_len < (size_t)n) {
        ++next_candidate;

        /* Skip obvious composites. */
        if (next_candidate > 2 && (next_candidate & 1) == 0) continue;
        if (next_candidate > 5 && (next_candidate % 5) == 0) continue;

        /* If the prime exceeds the bit‑width of seq_t, we cannot represent it. */
        if ((seq_t)next_candidate != next_candidate) continue;

        /* Check primality. */
        if (!is_prime(next_candidate)) continue;

        /* Digit filter. */
        if (!allowed_digits((seq_t)next_candidate)) continue;

        /* Store the found prime. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);          /* abort on allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)next_candidate;
    }
    return cache[(size_t)n - 1];
}
