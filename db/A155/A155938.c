/*
 * A155938  Primes p such that 13*p + 8 is also prime.
 * https://oeis.org/A155938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 108, u32 n <= 52, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth prime p such that 13*p+8 is also prime. */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache_seq = NULL;   /* cached sequence terms   */
static size_t cache_len = 0;      /* number of cached terms   */
static size_t cache_cap = 0;      /* allocated capacity       */
static seq_t next_candidate = 3;  /* next odd integer to test */

/* Return the nth term of OEIS sequence A155938 (1‑based index). */
seq_t a(idx_t n)
{
    /* Subsequent indices below the offset are undefined; return 0. */
    if (n < 1) return 0;

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Search for the next qualifying prime. */
        while (1) {
            if (is_prime_big(next_candidate)) {
                seq_t q = next_candidate * (seq_t)13 + (seq_t)8;
                if (is_prime_big(q)) {
                    /* Append to cache. */
                    if (cache_len == cache_cap) {
                        cache_cap = cache_cap ? cache_cap * 2 : 4;
                        cache_seq = (seq_t *)realloc(cache_seq,
                                                     cache_cap * sizeof(seq_t));
                    }
                    cache_seq[cache_len++] = next_candidate;
                    /* Move to the next candidate for future calls. */
                    next_candidate += (seq_t)2;
                    break;  /* Found one term, back to outer loop. */
                }
            }
            /* Not a qualifying prime – try the next odd integer. */
            next_candidate += (seq_t)2;
        }
    }

    return cache_seq[n - 1];
}
