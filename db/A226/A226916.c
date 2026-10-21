/*
 * A226916  Number of (17,11)-reverse multiples with n digits.
 * https://oeis.org/A226916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-2)+a(n-6)+b(n) * where b(4)=+1, b(6)=−1, b(7)=+1,
 *            b(8)=+1, otherwise 0. * This follows from the generating
 *            function * F(x)=x⁴(1+x)(1−x+x³)/(1−x²−x⁶). * * The sequence is
 *            computed by a simple O(n) recurrence and cached
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 123, u64 n <= 239,
 *            u128 n <= 473, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_realloc.h"   /* for seq_realloc() */

static seq_t *cache = NULL;      /* cache[0..max_seen] */
static size_t cache_size = 0;    /* number of elements stored in cache */

/* Ensure that cache holds values up to index n (inclusive).  */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_size > n) return;

    size_t old_size = cache_size;
    cache_size = (size_t)n + 1;
    cache = seq_realloc(cache, old_size, cache_size);

    /* Compute new entries using the recurrence. */
    for (size_t i = old_size; i < cache_size; ++i) {
        idx_t idx = (idx_t)i;
        seq_t val = 0;

        if (idx >= 2) val += cache[idx - 2];
        if (idx >= 6) val += cache[idx - 6];

        /* add the forcing term b(n) */
        if (idx == 4 || idx == 7 || idx == 8) val += 1;
        if (idx == 6)                      val -= 1;

        cache[i] = val;
    }
}

/* Public entry point required by the test harness. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
