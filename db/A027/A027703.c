/*
 * A027703  Primes with even number of 1's in binary expansion such that next
 *          prime has odd number of 1's.
 * https://oeis.org/A027703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p has an even number of 1’s in
 *            binary and the next prime after p has an odd number of 1’s
 * Range:     u8 n <= 12, u16 n <= 1650, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/is_prime.h"        /* bool is_prime(idx_t x); */
#include "../utils/seq_realloc.h"     /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap); */

/* Cache of the sequence values.  The array is grown only when more space is
   needed. */
static seq_t *term_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensures that 'term_cache' has room for at least 'needed' additional entries. */
static void ensure_cache_cap(size_t needed)
{
    if (cache_len + needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < cache_len + needed)
            new_cap = cache_len + needed;
        term_cache = seq_realloc(term_cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
}

/* Appends a value to the sequence cache. */
static void append_cache(seq_t val)
{
    ensure_cache_cap(1);
    term_cache[cache_len++] = val;
}

/* Returns the nth element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for 0, safe fallback */

    static uint64_t next_candidate = 2;   /* next integer to test for primality */
    static uint64_t previous_prime = 0;   /* last prime found */
    static unsigned int previous_popcount = 0;   /* popcount of previous_prime */

    /* Generate primes until the cache holds n elements. */
    while (cache_len < (size_t)n) {
        /* Test the current candidate for primality. */
        if (is_prime(next_candidate)) {
            /* Compute popcount of the current prime. */
            unsigned int pc_cur = __builtin_popcountll(next_candidate);

            /* If we already have a previous prime, check the parity condition. */
            if (previous_prime) {
                if ((previous_popcount & 1U) == 0U && (pc_cur & 1U) == 1U) {
                    /* Store the previous prime into the cache. */
                    append_cache((seq_t)(uint64_t)previous_prime);
                }
            }

            /* Update the previous prime information. */
            previous_prime     = next_candidate;
            previous_popcount = pc_cur;
        }

        /* Move to the next integer to test. */
        ++next_candidate;
    }

    return term_cache[(size_t)n - 1];
}
