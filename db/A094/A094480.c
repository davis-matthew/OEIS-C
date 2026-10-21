/*
 * A094480  Unhappy primes, primes that are also unhappy numbers.
 * https://oeis.org/A094480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code computes the sequence of unhappy primes by searching primes
   incrementally and testing each one for happiness. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t) */

static bool is_happy(seq_t x)
{
    while (true) {
        if (x == (seq_t)1) return true;
        if (x == (seq_t)4) return false;
        seq_t sum = (seq_t)0;
        seq_t t   = x;
        while (t > 0) {
            seq_t d = t % (seq_t)10;
            sum += d * d;
            t   /= (seq_t)10;
        }
        x = sum;
    }
}

/* Cache of found unhappy primes. */
static seq_t *unhappy_cache = NULL;
static size_t cache_len     = 0;
static idx_t next_candidate = 2;

/* Return the n-th unhappy prime (1‑based indexing). */
seq_t a(idx_t n)
{
    /* Expand the cache until it contains at least n entries. */
    while (cache_len < (size_t)n) {
        /* Find the next prime number starting at next_candidate. */
        idx_t p = next_candidate;
        while (!is_prime(p))
            ++p;

        /* Test if this prime is unhappy. */
        if (!is_happy((seq_t)p)) {
            /* Grow the cache and store the new unhappy prime. */
            seq_t *tmp = (seq_t *)realloc(unhappy_cache,
                                          (cache_len + 1) * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failure: return 0 as an error indicator. */
                return (seq_t)0;
            }
            unhappy_cache = tmp;
            unhappy_cache[cache_len] = (seq_t)p;
            ++cache_len;
        }

        /* Prepare for the next search. */
        next_candidate = p + 1;
    }

    return unhappy_cache[(size_t)(n - 1)];
}
