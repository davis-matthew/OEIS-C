/*
 * A062080  Group the even numbers as 2, (4,6), (8,10,12), (14,16,18,20),
 *          (22,24,26,28,30), ... then a(n) = LCM of the n-th group.
 * https://oeis.org/A062080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A062080: LCM of the n-th group of consecutive even numbers.
 * Group k contains k numbers: 2 + (k-1)k, 2 + (k-1)k + 2, ..., 2 + (k-1)k + 2(k-1).
 * a(k) is the least common multiple of that group.
 */

#include <stdlib.h>
#include "../utils/gcd_seq.h"   /* provides: seq_t gcd_seq(seq_t, seq_t) */

/* Compute lcm(a,b) using the provided gcd_seq. */
static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd_seq(a, b);     /* gcd(a,b) */
    return (a / g) * b;          /* lcm = a/g * b; overflow ok for 64‑bit builds */
}

/* lcm of the numbers in group k (k >= 1). */
static seq_t lcm_group(idx_t k)
{
    seq_t lcm = (seq_t)1;
    /* first number of the group: start = 2 + (k-1)*k  */
    idx_t tk   = (k - 1) * k;                   /* (k-1)*k fits in idx_t */
    seq_t start = (seq_t)2 + (seq_t)tk;         /* cast to seq_t */

    for (idx_t i = 0; i < k; ++i) {
        seq_t val = start + (seq_t)(2 * i);
        lcm = lcm_seq(lcm, val);
    }
    return lcm;
}

/* Global cache of previously computed terms. */
static seq_t *cache = NULL;      /* pointer to cached values; cache[0] unused */
static idx_t cache_size = 0;     /* number of valid entries in cache (>= 0) */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* offset is 1; behaviour for n < 1 is irrelevant */
        return (seq_t)0;
    }

    /* Grow the cache if necessary.  All cached values are computed for
       indices 1..cache_size. */
    if (cache_size < n) {
        size_t new_size = (size_t)n + 1; /* include index 0 */
        seq_t *tmp;
        if (cache == NULL) {
            tmp = (seq_t *)calloc(new_size, sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        if (tmp == NULL && new_size > cache_size) {
            /* Allocation failed – abort.  The build environment will
               handle such a failure. */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute missing terms.  cache[0] remains unused. */
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            cache[k] = lcm_group(k);
        }
        cache_size = n;
    }

    return cache[n];
}
