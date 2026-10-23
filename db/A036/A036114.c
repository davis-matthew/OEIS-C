/*
 * A036114  A summarize Fibonacci sequence starting with a(0)=3 and a(1)=3:
 *          summarize the previous two terms!.
 * https://oeis.org/A036114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   digit_counts, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Summary:
   a(0) = 3, a(1) = 3
   For n ≥ 2 the term a(n) is obtained by
   concatenating, for each digit d from 9 down to 0 that occurs in
   a(n‑1) or a(n‑2), the decimal representation of the total count
   of d in the two previous terms followed by the digit d itself.
   (Digits with zero count are omitted.) */

#include <stdlib.h>
#include "../utils/digit_counts.h"
#include "../utils/pow10.h"

/* cache for already computed terms */
static seq_t *cache      = NULL;
static size_t cache_size = 0;  /* number of stored terms (0..cache_size-1) */
static size_t cache_cap  = 0;  /* allocated capacity */

/* enlarge the cache to hold at least (n+1) terms */
static void ensure_cap(size_t n)
{
    if (cache_cap <= n) {
        size_t new_cap = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)3;

    /* Initialise cache on first use */
    if (cache_size == 0) {
        ensure_cap(1);
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)3;
        cache_size = 2;
    }

    /* If the requested term is already cached, return it */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Compute missing terms up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        unsigned char cnt[10] = {0};
        unsigned char tmp[10];

        /* Count digits in a(i-1) */
        digit_counts(cache[i-1], tmp);
        for (int d = 0; d < 10; ++d)
            cnt[d] += tmp[d];

        /* Count digits in a(i-2) */
        digit_counts(cache[i-2], tmp);
        for (int d = 0; d < 10; ++d)
            cnt[d] += tmp[d];

        /* Build the new term by concatenating count+digit pairs */
        seq_t result = 0;
        for (int d = 9; d >= 0; --d) {
            unsigned int c = cnt[d];
            if (c == 0) continue;          /* skip digits that don't appear */

            /* Determine number of decimal digits in c */
            unsigned int len = 0;
            unsigned int t = c;
            while (t > 0) { ++len; t /= 10; }
            if (len == 0) len = 1;          /* defensive, not needed */

            seq_t pow = pow10(len + 1);     /* 10^(len+1) */
            seq_t part = (seq_t)c * 10 + d; /* concatenate c and d */
            result = result * pow + part;
        }

        ensure_cap(i);
        cache[i] = result;
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}
