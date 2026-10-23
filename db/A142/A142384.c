/*
 * A142384  Primes congruent to 33 mod 47.
 * https://oeis.org/A142384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 139, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142384: primes congruent to 33 (mod 47)
   Computation method: iterate candidates of the form 47*k+33,
   test each for primality with is_prime_big(), and cache the
   results so subsequent calls for larger n do not redo work. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

static seq_t *prime_cache = NULL;
static size_t cache_cap  = 0;
static size_t cache_cnt  = 0;
static idx_t  next_k      = 2;     /* 47*2+33 = 127 is the first prime */

/* Ensure the cache has at least n primes. */
static void ensure_cached(idx_t n)
{
    if (cache_cnt >= (size_t)n)
        return;                                  /* already enough */

    /* Allocate an initial cache if uninitialized. */
    if (cache_cap == 0) {
        cache_cap = 32;
        prime_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_cnt < (size_t)n) {
        /* Generate next candidate in the residue class 33 (mod 47). */
        seq_t candidate = (seq_t)47 * (seq_t)next_k + (seq_t)33;
        ++next_k;

        /* Test for primality. */
        if (is_prime_big(candidate)) {
            /* Grow cache if necessary. */
            if (cache_cnt == cache_cap) {
                cache_cap *= 2;
                prime_cache = (seq_t *)realloc(prime_cache,
                                               cache_cap * sizeof(seq_t));
            }
            prime_cache[cache_cnt++] = candidate;
        }
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* sequence is undefined for offset < 1 */
    ensure_cached(n);                /* make sure the n‑th element is cached */
    return prime_cache[(size_t)(n - 1)];
}
