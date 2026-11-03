/*
 * A078561  p, p+4 and p+10 are consecutive primes.
 * https://oeis.org/A078561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 243, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 910 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A078561
   a(n) is the n‑th prime p such that p, p+4, p+10 are consecutive primes.
   (i.e. next_prime(p) == p+4 and next_prime(p+4) == p+10) */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* static cache for the sequence values */
static seq_t *cache     = NULL;  /* array of found primes */
static size_t cacheSize = 0;     /* number of values stored   */
static size_t cacheCap  = 0;     /* allocated capacity        */

seq_t a(idx_t n)
{
    /* offset 1 – a(1) is the first term. For n==0 return 0. */
    if (n == 0)
        return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        grow_seq(&cache, &cacheCap, (size_t)16);
    }

    /* generate terms until we have at least n of them */
    uint64_t current_prime = 0; /* start below the first prime */
    while (cacheSize < (size_t)n) {
        current_prime = next_prime((uint64_t)current_prime); /* next prime p     */
        seq_t p = (seq_t)current_prime;

        /* check first gap */
        seq_t next_p = (seq_t)next_prime((uint64_t)p);
        if (next_p == p + (seq_t)4) {
            /* check second gap */
            seq_t next_next_p = (seq_t)next_prime((uint64_t)next_p);
            if (next_next_p == p + (seq_t)10) {
                /* store the prime if it wasn't already stored
                   (duplicate occurs when re‑scanning over a larger limit) */
                if (cacheSize == 0 || p > cache[cacheSize - 1]) {
                    grow_seq(&cache, &cacheCap, cacheSize + 1);
                    cache[cacheSize++] = p;
                }
            }
        }
    }

    return cache[n - 1];
}
