/*
 * A154767  Primes with even largest digit.
 * https://oeis.org/A154767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1732, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime whose largest decimal digit is even. */
/* The function uses a simple trial‑division primality test and
   caches previously found primes so that calls are independent of order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Forward declaration from the build configuration. */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Helper: trial‑division primality test for unsigned 64‑bit numbers. */
/* ------------------------------------------------------------------ */
static bool is_prime_uint64(uint64_t num)
{
    if (num < 2)               return false;
    if (num % 2 == 0)          return num == 2;
    for (uint64_t d = 3; d <= num / d; d += 2) {
        if (num % d == 0)      return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  Cache is maintained statically.           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Cache of primes that satisfy the property. */
    static seq_t *cache      = NULL;   /* array of found primes */
    static size_t cache_len  = 0;      /* number of cached terms   */
    static size_t cache_cap  = 0;      /* allocated capacity       */

    /* Next integer to test for primality: start from 2. */
    static uint64_t next_candidate = 2;

    /* Ignore indices below the OEIS offset (1). */
    if (n <= 0) return (seq_t)0;

    /* Return from cache if already available. */
    if (n <= cache_len) return cache[n - 1];

    /* Allocate minimal initial capacity. */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;  /* should not happen */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t p = (seq_t)next_candidate;      /* prime candidate */

        /* Advance next_candidate for the next iteration. */
        if (next_candidate == 2)                 /* after 2 we test 3,5,7,... */
            next_candidate = 3;
        else
            next_candidate += 2;                 /* skip even numbers */

        /* Primality test on the candidate. */
        if (is_prime_uint64((uint64_t)p)) {
            /* Compute largest decimal digit of p. */
            uint64_t tmp  = (uint64_t)p;
            uint64_t maxd = 0;
            while (tmp > 0) {
                uint64_t d = tmp % 10;
                if (d > maxd)   maxd = d;
                tmp /= 10;
            }
            /* Keep the prime if the largest digit is even. */
            if ((maxd & 1) == 0) {
                /* Grow the cache if necessary. */
                if (cache_len >= cache_cap) {
                    cache_cap <<= 1;               /* double capacity */
                    seq_t *new_ptr = (seq_t *)
                        realloc(cache, sizeof(seq_t) * cache_cap);
                    if (!new_ptr) {                 /* allocation failure */
                        free(cache);
                        cache = NULL; cache_cap = 0;
                        return (seq_t)0;
                    }
                    cache = new_ptr;
                }
                cache[cache_len++] = p;
            }
        }
    }

    return cache[n - 1];
}
