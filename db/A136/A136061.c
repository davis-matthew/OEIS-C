/*
 * A136061  Primes p such that (p+4)/5 is also prime.
 * https://oeis.org/A136061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that (p+4)/5 is also prime
 * Range:     u8 n <= 6, u16 n <= 269, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;          /* cached terms of the sequence */
static size_t cache_len = 0;         /* number of cached terms */
static size_t cache_cap = 0;         /* capacity of the cache array */
static uint64_t next_q = 2;          /* next candidate for q = (p+4)/5 */

/* Append a term to the cache, growing it if necessary */
static void append_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap * 2;
        ensure_capacity(new_cap, &cache_cap, (void **)&cache, sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Return the n-th term (n >= 1) of sequence A136061 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range, undefined */

    /* build terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Find the next prime q */
        while (!is_prime(next_q)) {
            next_q += (next_q % 2 == 0 ? 1 : 2);
        }
        uint64_t q = next_q;
        /* Compute candidate p = 5*q - 4 */
        uint64_t p = 5 * q - 4;
        if (is_prime(p)) {
            append_term((seq_t)p);
        }
        /* Advance to the next odd candidate */
        next_q += (next_q % 2 == 0 ? 1 : 2);
    }

    return cache[n - 1];
}
