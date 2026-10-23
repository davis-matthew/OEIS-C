/*
 * A158238  Primes p such that (p-7)/8 and 8p + 7 are both prime.
 * https://oeis.org/A158238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p such that (p‑7)/8 and 8p+7 are both
 *            prime. Offset: 1 (a(1) is the first such prime)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *seq_cache = NULL;          /* cached terms */
static size_t cache_len  = 0;            /* number of stored terms */
static size_t cache_cap  = 0;            /* allocated capacity */
static seq_t next_candidate = 7;         /* next number ≡ 7 (mod 8) to test */

seq_t a(idx_t n)
{
    /* n ≥ 1 by the OEIS offset, but handle n = 0 gracefully */
    if (n == 0) return 0;

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* The candidate itself must be prime. */
        if (!isPrimeGeneric(next_candidate)) {
            next_candidate += 8;         /* only numbers ≡ 7 (mod 8) can be candidates */
            continue;
        }

        /* Check that (p‑7)/8 is prime. */
        seq_t q1 = (next_candidate - 7) / 8;
        if (!isPrimeGeneric(q1)) {
            next_candidate += 8;
            continue;
        }

        /* Check that 8p+7 is prime. */
        seq_t q2 = (next_candidate * 8) + 7;
        if (!isPrimeGeneric(q2)) {
            next_candidate += 8;
            continue;
        }

        /* All conditions satisfied – store the term. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE); /* out of memory */
            seq_cache = tmp;
            cache_cap = new_cap;
        }
        seq_cache[cache_len++] = next_candidate;

        /* Prepare next candidate. */
        next_candidate += 8;
    }

    return seq_cache[n - 1];
}
