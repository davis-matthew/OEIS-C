/*
 * A107301  Numbers k such that 10007*k + 99991 is prime.
 * https://oeis.org/A107301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 3421, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9069 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Values k such that 10007*k + 99991 is prime.
   The function searches for successive ks, tests each one for
   primality (using is_prime), and caches the results so that
   subsequent calls do not repeat work. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

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

        if (is_prime_u64(val)) {
            /* Store the discovered k. */
            cache[cache_size++] = (seq_t)next_k;
        }

        ++next_k;
    }

    return cache[n - 1];
}
