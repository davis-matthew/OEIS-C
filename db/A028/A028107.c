/*
 * A028107  Expansion of 1/((1-3x)(1-9x)(1-11x)(1-12x)).
 * https://oeis.org/A028107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Expansion of 1/((1-3x)(1-9x)(1-11x)(1-12x)) Recurrence (n ≥ 4):
 *            a(n) = 35*a(n-1) - 435*a(n-2) + 2205*a(n-3) - 3564*a(n-4)
 *            Initial values: a(0)=1, a(1)=35, a(2)=790, a(3)=14630
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdint.h>   /* uint64_t definition for idx_t if needed */

static seq_t *cache = NULL;          /* memoized terms */
static idx_t  cache_val_count = 0;   /* number of terms initialized (0 … cache_val_count-1) */
static size_t cache_capacity = 0;    /* allocated array size */

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_capacity)
        return;
    size_t new_cap = needed <= 2 ? 2 : needed;
    while (new_cap < needed)
        new_cap <<= 1;                 /* grow by doubling */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp)
        abort();                       /* out‑of‑memory: terminate */
    cache = tmp;
    cache_capacity = new_cap;
}

/* Compute the n-th term of A028107 */
seq_t a(idx_t n)
{
    /* Fast path for already computed values */
    if (n < cache_val_count)
        return cache[n];

    /* Initialise base cases if this is the first call */
    if (cache_val_count == 0) {
        ensure_capacity(4);
        cache[0] = 1;
        cache[1] = 35;
        cache[2] = 790;
        cache[3] = 14630;
        cache_val_count = 4;
    }

    /* Compute missing values up to n */
    if (n >= cache_val_count) {
        ensure_capacity((size_t)n + 1);   /* ensure storage for indices 0…n */
        for (idx_t i = cache_val_count; i <= n; ++i) {
            /* Recurrence: a(i) = 35*a(i-1) - 435*a(i-2) + 2205*a(i-3) - 3564*a(i-4) */
            seq_t t1 = 35 * cache[(size_t)(i - 1)];
            seq_t t2 = 2205 * cache[(size_t)(i - 3)];
            seq_t t3 = 435 * cache[(size_t)(i - 2)];
            seq_t t4 = 3564 * cache[(size_t)(i - 4)];
            /* sum of the positive terms */
            seq_t val = t1 + t2;
            /* subtract the negative terms (the sequence ensures the result stays non‑negative) */
            val -= t3;
            val -= t4;
            cache[(size_t)i] = val;
        }
        cache_val_count = (idx_t)n + 1;
    }

    return cache[n];
}
