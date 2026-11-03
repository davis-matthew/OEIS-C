/*
 * A106014  Primes p such that 100*p +- 3 are primes.
 * https://oeis.org/A106014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 371, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9527 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search over primes p such that 100*p ± 3 are primes */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Static cache for computed terms */
static seq_t *seq_cache = NULL;   /* array of a(k) values */
static idx_t seq_cap  = 0;        /* current capacity of seq_cache */
static idx_t seq_len  = 0;        /* number of values stored in seq_cache */

/* Ensure the cache has at least `min_cap` elements */
static void ensure_seq_cap(idx_t min_cap)
{
    if (seq_cap < min_cap)
    {
        idx_t new_cap = seq_cap ? seq_cap * 2 : 32;
        if (new_cap < min_cap)
            new_cap = min_cap;
        seq_t *new_ptr = mem_resize(seq_t, seq_cache, new_cap);
        if (!new_ptr) /* allocation failure */
            exit(EXIT_FAILURE);
        seq_cache = new_ptr;
        seq_cap   = new_cap;
    }
}

/* Append a new value to the cache */
static void push_seq_val(seq_t val)
{
    ensure_seq_cap(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* Main function returning the nth element (1‑based indexing) */
seq_t a(idx_t n)
{
    if (n == 0) /* safety for 0 index; sequence offset is 1 */
        return 0;

    /* If we already have the requested term, return it */
    if (seq_len >= n)
        return seq_cache[n - 1];

    /* Static variable to remember the next candidate integer */
    static uint64_t next_candidate = 2;
    const uint64_t factor = 100ULL;

    /* Search until we have found the nth term */
    while (seq_len < n)
    {
        /* Skip even numbers greater than 2 */
        if (next_candidate > 2 && (next_candidate & 1ULL) == 0)
        {
            ++next_candidate;
            continue;
        }

        /* Test candidate for primality */
        if (is_prime_u64(next_candidate))
        {
            uint64_t aux1 = factor * next_candidate - 3ULL;
            uint64_t aux2 = factor * next_candidate + 3ULL;

            if (is_prime_u64(aux1) && is_prime_u64(aux2))
            {
                /* Store the prime in the cache */
                push_seq_val((seq_t)next_candidate);
            }
        }

        ++next_candidate;
    }

    return seq_cache[n - 1];
}
