/*
 * A141452  a(1)=1, a(n) = the smallest integer > a(n-1) that is divisible by
 *          the number of divisors of a(n-1).
 * https://oeis.org/A141452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(n) is the smallest integer >a(n‑1) divisible by the
 *            number of divisors of a(n‑1)
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Compute the number of positive divisors of n.  Works for all seq_t
   sizes supported by the build.  The algorithm is a simple trial
   division that stops at sqrt(n). */
static seq_t divisor_count(seq_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t count = 1;
    seq_t remainder = n;
    seq_t limit = floor_sqrt(remainder);

    /* Factor out 2 separately to reduce loop iterations. */
    if (remainder % (seq_t)2 == 0) {
        seq_t exp = 0;
        while (remainder % (seq_t)2 == 0) {
            remainder /= (seq_t)2;
            ++exp;
        }
        count *= (exp + (seq_t)1);
        limit = floor_sqrt(remainder);
    }

    /* Trial divide by odd numbers up to sqrt(remainder). */
    for (uint64_t i = 3; (seq_t)i <= limit; i += 2) {
        seq_t p = (seq_t)i;
        if (remainder % p == 0) {
            seq_t exp = 0;
            while (remainder % p == 0) {
                remainder /= p;
                ++exp;
            }
            count *= (exp + (seq_t)1);
            limit = floor_sqrt(remainder);
        }
    }

    /* If a prime factor larger than sqrt remains. */
    if (remainder > (seq_t)1)
        count *= (seq_t)2;

    return count;
}

/* Cache for computed terms.  cache[0] holds a(1). */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_size = 0;

/* Ensure cache has enough capacity for at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (cache == NULL) {
        cache_cap = needed > 16 ? needed : 16;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
    } else if (needed > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed)
            new_cap = needed;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n < 1, safe return */

    /* Lazy initialization of cache.  First term a(1) = 1. */
    if (cache == NULL) {
        cache_cap   = 16;
        cache       = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        cache[0]    = (seq_t)1;
        cache_size  = 1;
    }

    /* Extend cache up to required index. */
    while (cache_size < (size_t)n) {
        seq_t prev = cache[cache_size - 1];
        seq_t d    = divisor_count(prev);
        seq_t next = ((prev / d) + (seq_t)1) * d;
        ensure_cache_cap(cache_size + 1);
        cache[cache_size++] = next;
    }

    return cache[n - 1];
}
