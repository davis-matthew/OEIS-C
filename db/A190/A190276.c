/*
 * A190276  Numbers k such that tau(2k-1) = tau(2k+1) where tau(k) =
 *          A000005(k).
 * https://oeis.org/A190276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th natural number k such that tau(2k‑1) =
 *            tau(2k+1), where tau(m) is the number of divisors of m
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"   /* provides divisor_count_u64 */

/* Dynamic cache of sequence values. */
static seq_t *cache = NULL;      /* array of computed a(i) */
static size_t cache_len = 0;     /* how many values stored */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static uint64_t last_checked = 0;/* last k processed (k >= 1) */

/* Grow the cache to hold at least 'size' elements. */
static void cache_grow(size_t size)
{
    if (size <= cache_cap)
        return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < size)
        new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp)
        abort();                 /* out of memory */
    cache = newp;
    cache_cap = new_cap;
}

/* Return the n-th term of OEIS sequence A190276. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* offset 1: a(1) is first term */

    size_t target = (size_t)n;        /* need at least this many terms */

    /* Generate terms until we reach the requested index. */
    while (cache_len < target) {
        ++last_checked;               /* candidate k */

        uint64_t x = (last_checked << 1) - 1;   /* 2k-1 */
        uint64_t y = x + 2;                 /* 2k+1 */

        if (divisor_count_u64(x) == divisor_count_u64(y)) {
            cache_grow(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[target - 1];   /* zero‑based indexing */
}
