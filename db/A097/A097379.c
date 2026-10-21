/*
 * A097379  Numbers m such that 1+SquareFreeKernel(m) is prime.
 * https://oeis.org/A097379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer m such that 1 + squarefree_kernel(m) is
 *            prime
 * Range:     u8 n <= 77, u16 n <= 6330, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches sequentially, caching results. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_len = 0;         /* number of values currently cached */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static uint64_t next_candidate = 1;  /* next integer to be tested */

/* Grow the cache to accommodate at least `needed` entries. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) {
        /* Allocation failed – abort; in typical environments this is fatal. */
        abort();
    }
    cache = new_buf;
    cache_cap = new_cap;
}

/* Return the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) /* Not part of OEIS; return 0 for safety. */
        return 0;

    /* Extend cache until it contains at least n elements. */
    while (cache_len < (size_t)n) {
        uint64_t m = next_candidate;

        /* Compute the square‑free kernel of m by multiplying the
           distinct prime factors returned by factor_u64. */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t pcnt;
        factor_u64(m, primes, exps, &pcnt);

        seq_t kernel = 1;
        for (size_t i = 0; i < pcnt; ++i)
            kernel = kernel * (seq_t)primes[i];

        /* If kernel+1 is prime, store m in the cache. */
        if (is_prime_big(kernel + (seq_t)1)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)m;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
