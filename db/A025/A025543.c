/*
 * A025543  Least common multiple of the first n composite numbers.
 * https://oeis.org/A025543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 24, u64 n <= 61, u128 n <= 126,
 *            big any n
 * Helpers:   gcd, is_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least common multiple of first n composite numbers. */

#include <stddef.h>   /* size_t, NULL */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc, abort */

#include "../utils/is_prime.h"
#include "../utils/gcd.h"
#include "../utils/seq_mul.h"

/* Static cache for already computed values */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_size = 0;
static seq_t current_lcm = 1;    /* LCM of composites seen so far */
static uint64_t last_comp  = 3;  /* last integer examined for compositeness */

/* Ensure the cache has capacity for at least |need| elements */
static void
ensure_cache(size_t need)
{
    if (need > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? need + 1 : cache_cap * 2 + 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (!cache) {
        cache_cap  = 1;
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
        cache[0] = (seq_t)1;          /* a(0) = 1 by definition */
    }

    /* Return already computed value if available */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Extend cache up to n */
    while (cache_size <= (size_t)n) {
        /* Find next composite number */
        uint64_t num = ++last_comp;
        while (is_prime(num)) num = ++last_comp;
        last_comp = num;

        /* LCM update: lcm = current_lcm / gcd(current_lcm,num) * num */
        seq_t g     = gcd(current_lcm, (seq_t)num);
        seq_t tmp   = current_lcm / g;
        current_lcm = seq_mul(tmp, (seq_t)num);

        /* Store new value */
        ensure_cache(cache_size + 1);
        cache[cache_size] = current_lcm;
        ++cache_size;
    }

    return cache[(size_t)n];
}
