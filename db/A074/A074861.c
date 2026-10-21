/*
 * A074861  Iccanobirt sequence: a(n) = R(a(n-1)) + R(a(n-2)) + R(a(n-3))
 *          where a(1)=a(2)=a(3)=1 and R(n) (A004086) is the reverse of n.
 * https://oeis.org/A074861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(1)=a(2)=a(3)=1, a(n)=R(a(n-1))+R(a(n-2))+R(a(n-3)) for n≥4,
 *            where R(x) is the decimal reverse of x
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 28, u64 n <= 61, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Reverse a decimal number. Works for any seq_t that supports
   the usual integer operators. */
static seq_t reverse_decimal(seq_t v)
{
    seq_t r = 0;
    while (v > 0) {
        r = r * 10 + (v % 10);
        v /= 10;
    }
    return r;
}

/* Compute a(n).  Uses memoisation; previous values are never
   recomputed.  Works for any order of calls. */
seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i).  Index 0 is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure the cache can hold indices up to n. */
    if (n >= cache_size) {
        idx_t new_size = cache_size == 0 ? 4 : cache_size;
        while (new_size <= n)
            new_size <<= 1;                /* grow exponentially */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed: return 0 as a sentinel. */
            return 0;
        }
        cache = new_cache;
        cache_size = new_size;
        /* Initialise newly added entries to 0. */
        for (idx_t i = cache_size / 2; i < cache_size; ++i) cache[i] = 0;
    }

    /* Base cases. */
    if (n == 1 || n == 2 || n == 3) {
        cache[n] = 1;
        return cache[n];
    }

    /* If already computed, return. */
    if (cache[n] != 0)
        return cache[n];

    /* Compute missing values up to n. */
    for (idx_t i = 3; i < n; ++i) {
        if (cache[i + 1] == 0) { /* a(i+1) not yet computed */
            seq_t r1 = reverse_decimal(cache[i]);   /* a(i) */
            seq_t r2 = reverse_decimal(cache[i - 1]);/* a(i-1) */
            seq_t r3 = reverse_decimal(cache[i - 2]);/* a(i-2) */
            cache[i + 1] = r1 + r2 + r3;
        }
    }

    return cache[n];
}
