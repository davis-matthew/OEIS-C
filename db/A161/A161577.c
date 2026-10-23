/*
 * A161577  a(1)=2. a(n) = the smallest integer > a(n-1) such that d(a(n)) >=
 *          d(n), where d(n) = the number of divisors of n.
 * https://oeis.org/A161577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 147, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the smallest integer > a(n‑1) such that d(a(n)) >= d(n),
 * where a(1) = 2 and d(m) is the divisor count of m.
 * This implementation memoises values and grows the cache on demand.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"

static seq_t *cache = NULL;        /* sequence values, 0‑based */
static size_t cached_len = 0;      /* number of elements stored */
static size_t cached_cap = 0;      /* capacity of the cache buffer */

/* Grow the cache to at least `needed` capacity. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cached_cap)
        return;
    size_t new_cap = cached_cap ? cached_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) {
        /* In a real project you'd prefer a proper error handling. */
        exit(EXIT_FAILURE);
    }
    cache = new_buf;
    cached_cap = new_cap;
}

/* Compute the sequence up to index `n` (1‑based). */
static void ensure_seq_upto(idx_t n)
{
    if (n <= cached_len)
        return;                    /* already computed */

    if (cached_len == 0) {          /* initialise with a(1) = 2 */
        ensure_cache_cap(1);
        cache[0] = (seq_t)2;
        cached_len = 1;
    }

    for (idx_t i = cached_len + 1; i <= n; ++i) {
        uint64_t d_i = divisor_count_u64((uint64_t)i);  /* d(n) */

        seq_t candidate = cache[cached_len - 1] + 1;      /* > a(n‑1) */
        while (divisor_count_u64((uint64_t)candidate) < d_i)
            ++candidate;

        ensure_cache_cap(cached_len + 1);
        cache[cached_len++] = candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n < 1, return 0 for safety */
        return (seq_t)0;
    ensure_seq_upto(n);
    return cache[n - 1];
}
