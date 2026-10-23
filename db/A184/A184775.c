/*
 * A184775  Numbers k such that floor(k*sqrt(2)) is prime.
 * https://oeis.org/A184775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 1837, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A184775: the nth positive integer k such that floor(k*sqrt(2)) is prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"

/* Cache for computed terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 1;      /* next k to test */

/* Ensure the cache has at least `need` slots. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);   /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute the sequence value. */
seq_t a(idx_t n)
{
    if (n == 0)              /* offset is 1; behaviour for n<1 is undefined */
        return 0;

    /* Allocate cache if not already done. */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(1);
    }

    /* Extend cache until we have the n‑th term. */
    while (cache_len < (size_t)n) {
        /* Compute floor(k*sqrt(2)) exactly as floor_sqrt(2*k*k). */
        seq_t kk = (seq_t)next_k;
        seq_t m = kk * kk;    /* k^2 */
        m *= (seq_t)2;        /* 2*k^2 */
        seq_t floor_val = floor_sqrt(m);

        /* Test primality. */
        if (is_prime_big(floor_val)) {
            /* Store the k value. */
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = kk;
        }
        ++next_k;
    }

    return cache[n - 1];
}
