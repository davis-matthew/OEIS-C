/*
 * A099609  Naive list of twin primes (A077800 prefixed by 2, 3).
 * https://oeis.org/A099609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1722, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A099609
   Naive list of twin primes (A077800 prefixed by 2, 3).
   The sequence begins 2, 3, 3, 5, 5, 7, 11, 13, 17, 19, 29, 31, 41, 43, … */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

static seq_t  cache[20000];   /* room for more than 10 000 terms */
static size_t cache_len  = 0; /* number of terms already cached */

/* state for prime generation */
static uint64_t prev_prime    = 0;  /* last prime seen */
static uint64_t next_candidate = 5; /* next odd number to test */

/* Ensure cache contains at least n elements (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;            /* already have enough */

    /* first time: install the fixed prefix 2, 3 */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache_len = 2;
        prev_prime = 3;                     /* last prime in the prefix */
        next_candidate = 5;                 /* next odd to test */
    }

    /* keep generating primes until we have enough twin‑prime entries */
    while (cache_len < (size_t)n) {
        /* advance to the next prime */
        while (!is_prime(next_candidate))
            next_candidate += 2;
        uint64_t p = next_candidate;
        next_candidate += 2;                /* prepare for the next test */

        /* if p is twin to prev_prime, output both numbers */
        if (p - prev_prime == 2) {
            cache[cache_len++] = (seq_t)prev_prime; /* duplicate previous prime */
            cache[cache_len++] = (seq_t)p;         /* current prime */
        }

        prev_prime = p;                     /* new last prime */
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset is 1, safety for 0 */
    ensure_cache(n);
    return cache[n - 1];
}
