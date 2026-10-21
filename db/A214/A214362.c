/*
 * A214362  Arithmetic mean of next a(n) successive positive integers is
 *          prime.
 * https://oeis.org/A214362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 3565, u32 n <= 3566, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3; for n≥2, a(n) is the smallest odd k such that
   the arithmetic mean of the next k positive integers starting at the
   current block start is prime.  The sequence begins 3,3,1,7,5,7,... */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache          = NULL;   /* cache[0] holds a(1), … */
static size_t cache_size     = 0;      /* number of terms already computed */
static size_t cache_capacity = 0;      /* allocated capacity of cache   */
static seq_t next_start      = 1;      /* start of the next block        */

/* Grow the cache so that it can hold at least indices up to n (0‑based). */
static void ensure_cache_size(size_t n)
{
    if (n < cache_size)
        return;
    size_t new_cap = cache_capacity;
    if (new_cap == 0)
        new_cap = 16;
    while (new_cap <= n)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) { /* unlikely, but keep a sane state */
        cache_size = 0;
        cache_capacity = 0;
        cache = NULL;
        return;
    }
    cache = tmp;
    cache_capacity = new_cap;
}

/* Return the nth term of the sequence; n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n==0, harmless */

    size_t target = (size_t)n - 1; /* convert to 0‑based index */

    /* Extend the cache until we have computed up to target. */
    if (target >= cache_size) {
        ensure_cache_size(target);
        for (size_t i = cache_size; i <= target; ++i) {
            /* Find the smallest odd block size k such that
               next_start + (k-1)/2 is prime. */
            seq_t t = 0;            /* t = (k-1)/2 */
            while (1) {
                seq_t avg = next_start + t;
                if (is_prime_big(avg)) {
                    seq_t k = (t << 1) + 1;
                    cache[i] = k;
                    next_start += k;
                    break;
                }
                ++t;
            }
        }
        cache_size = target + 1;
    }

    return cache[target];
}
