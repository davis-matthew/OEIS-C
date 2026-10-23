/*
 * A143757  Aliquot sequence starting at 82.
 * https://oeis.org/A143757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Aliquot sequence starting at 82.
   Formula: a(0) = 82, a(n+1) = A001065(a(n))   (sum of proper divisors) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

static seq_t proper_divisor_sum(seq_t x)
{
    if (x <= 1) return 0;                /* 0 and 1 have no proper divisors */

    seq_t total = 1;                     /* 1 is always a proper divisor */
    seq_t sqrt_x = floor_sqrt(x);        /* integer sqrt of x */

    for (idx_t d = 2; d <= sqrt_x; ++d) {
        if (x % (seq_t)d == 0) {
            seq_t q = x / (seq_t)d;
            total += (seq_t)d;
            if (q != (seq_t)d) total += q;
        }
    }
    return total;
}

static seq_t *cache = NULL;      /* pointer to cached sequence values   */
static size_t cache_len = 0;      /* number of values stored so far      */
static size_t cache_cap = 0;      /* capacity of the cache buffer        */

static void grow_cache(idx_t n)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)82;      /* a(0) */
        cache_len = 1;
    }

    while (cache_cap <= (size_t)(n + 1)) cache_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
}

seq_t a(idx_t n)
{
    grow_cache(n);

    /* Compute any missing values up to index n */
    while (cache_len <= (size_t)n) {
        seq_t prev = cache[cache_len - 1];
        cache[cache_len] = proper_divisor_sum(prev);
        ++cache_len;
    }

    return cache[n];
}
