/*
 * A107301  Numbers k such that 10007*k + 99991 is prime.
 * https://oeis.org/A107301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 3421, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Values k such that 10007*k + 99991 is prime.
   The function searches for successive ks, tests each one for
   primality (using is_prime), and caches the results so that
   subsequent calls do not repeat work. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

/* mutable state shared across calls */
static seq_t  *cache      = NULL;   /* stored k values */
static idx_t   cache_size = 0;      /* number of terms currently cached */
static idx_t   cache_cap  = 0;      /* allocated capacity of cache array */
static uint64_t next_k    = 0;      /* next candidate k to test */

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* The offset of the sequence is 1; any call with n == 0 is
       considered an error; we simply return 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Find at least n valid k values, enlarging the cache as needed. */
    while (cache_size < n) {
        /* Grow the array when it is full. */
        if (cache_size == cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = seq_realloc(cache, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Check primality of 10007*next_k + 99991. */
        __uint128_t prod = (__uint128_t)10007 * next_k + 99991;
        uint64_t val = (uint64_t)prod;   /* safe for required range */

        if (is_prime(val)) {
            /* Store the discovered k. */
            cache[cache_size++] = (seq_t)next_k;
        }

        ++next_k;
    }

    return cache[n - 1];
}
