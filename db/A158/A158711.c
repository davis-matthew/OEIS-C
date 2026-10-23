/*
 * A158711  Primes p such that p1=Floor[p/2]+p is prime and p2=Floor[p1/2]+p1
 *          is prime.
 * https://oeis.org/A158711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p such that p1 = floor(p/2) + p is prime,
 *            and p2 = floor(p1/2) + p1 is prime. The code builds the terms on
 *            demand, storing them in a static dynamic array
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

static seq_t *cache   = NULL;   /* a(1), a(2), … stored sequentially */
static size_t cache_len = 0;    /* number of cached terms */
static size_t cache_cap = 0;    /* capacity of the cache array */
static seq_t next_candidate = (seq_t)3;   /* next odd integer to test */

/* Append a newly found term to the cache, growing the array if needed. */
static void cache_append(seq_t value)
{
    if (cache_len == cache_cap) {
        /* Request a new capacity that can hold one more element. */
        grow_seq(&cache, &cache_cap, cache_len + 1);
    }
    cache[cache_len++] = value;
}

/* Ensure that at least n terms are cached. */
static void ensure_terms(idx_t n)
{
    /* Work in size_t (the cache length type) for the comparison. */
    while (cache_len < (size_t)n) {
        if (is_prime_big(next_candidate)) {
            seq_t p1 = (next_candidate / (seq_t)2) + next_candidate;
            seq_t p2 = (p1 / (seq_t)2) + p1;
            if (is_prime_big(p1) && is_prime_big(p2)) {
                cache_append(next_candidate);
            }
        }
        next_candidate += (seq_t)2;   /* skip even numbers */
    }
}

/* Main sequence access function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* Offset is 1; a(0) is unused. */
    ensure_terms(n);
    return cache[n - 1];
}
