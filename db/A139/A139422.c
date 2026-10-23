/*
 * A139422  a(1)=a(2)=1. For n >= 3, a(n) = a(n-1) + d(a(n-1)) + d(a(n-2)),
 *          where d(m) is the number of positive divisors of m.
 * https://oeis.org/A139422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + d(a(n-1)) + d(a(n-2)), a(1)=a(2)=1 d(m) is the
 *            number of positive divisors of m
 * Range:     u8 n <= 19, u16 n <= 2720, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

/* Memoization state */
static seq_t *cache = NULL;       /* array of computed terms */
static size_t cache_size = 0;     /* allocated size (indices 1 … cache_size-1 are valid) */
static idx_t last_computed = 0;   /* largest index whose value is known */

/* Helper: return the divisor count of a value that fits in uint64_t.
   seq_t may be larger, but the recurrence never exceeds 64 bits for n up to
   10 000. */
static inline seq_t divisor_count(seq_t x)
{
    return (seq_t)divisor_count_u64((uint64_t)x);
}

seq_t a(idx_t n)
{
    /* Handle the two defined base cases directly. */
    if (n == 1 || n == 2)
        return (seq_t)1;

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) /* allocation failed – let the caller handle the OOM. */
            return (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise base values once. */
    if (last_computed == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        last_computed = 2;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t d1 = divisor_count(prev1);
        seq_t d2 = divisor_count(prev2);
        cache[i] = prev1 + d1 + d2;
    }
    last_computed = n;

    return cache[n];
}
