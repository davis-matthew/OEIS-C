/*
 * A008365  13-rough numbers: positive integers that have no prime factors
 *          less than 13.
 * https://oeis.org/A008365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 13‑rough numbers (OEIS A008365).
 * The nth term is the nth positive integer that has no prime factors < 13.
 * We generate the sequence by testing divisibility by the primes 2,3,5,7,11.
 * No heavy sieving is needed because only five small primes must be excluded.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of elements stored */
static seq_t  last_val = 0;      /* last generated rough number */

static void grow_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
}

static void compute_to(size_t up_to)
{
    while (cache_len < up_to) {
        /* Find next candidate > last_val that is not divisible by 2,3,5,7,11 */
        seq_t cand = last_val + 1;
        while (cand % 2 == 0 || cand % 3 == 0 || cand % 5 == 0 ||
               cand % 7 == 0 || cand % 11 == 0) {
            cand++;
        }
        /* Ensure enough space and store */
        grow_cache(cache_len + 1);
        cache[cache_len++] = cand;
        last_val = cand;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* undefined for n < 1 */
    size_t target = (size_t)n;
    compute_to(target);
    return cache[target - 1];
}
