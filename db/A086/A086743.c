/*
 * A086743  Numbers n such that the coefficient of x^n equals 0 in
 *          Product_{k>=1} (1 - x^(3^k)).
 * https://oeis.org/A086743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer that cannot be expressed as a
 *            sum of distinct powers of 3 (i.e. 3^k with k ≥ 1)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   zero_coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/zero_coeff.h"

/* Cache of the sequence values.  Values are kept as uint64_t
   because the nth term for n ≤ 10000 easily fits in 64 bits. */
static uint64_t *cache = NULL;     /* array of found numbers */
static size_t cache_len = 0;       /* number of stored terms   */
static size_t cache_cap = 0;       /* capacity of cache        */
static uint64_t next_search = 1;   /* next integer to test    */

/* Grow the cache array */
static void grow_cache(void)
{
    if (cache_cap == 0)
        cache_cap = 1024;
    else
        cache_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(cache, cache_cap * sizeof(uint64_t));
    cache = tmp;
}

/* Main function required by the harness.  Builds cache lazily. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1, return 0 for n=0 */
        return (seq_t)0;

    /* extend cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* find next number with zero coefficient */
        while (true) {
            if (zero_coeff(next_search)) {
                /* add to cache */
                if (cache_len == cache_cap)
                    grow_cache();
                cache[cache_len++] = (uint64_t)next_search;
                break;
            }
            ++next_search;
        }
        ++next_search;  /* prepare for next search */
    }

    return (seq_t)cache[n - 1];
}
