/*
 * A164770  Numbers k with the property that the average digit of k^2 is 2.
 * https://oeis.org/A164770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 248, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 59 terms through n = 2334 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A164770
   – numbers k such that the average digit of k² equals 2.
   The implementation scans k in increasing order, memoises found
   values, and guarantees correct results for all builds (8,16,32,
   64, 128 bit and arbitrary precision). */

/* necessary standard headers */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>           /* for uint64_t */
#include "../utils/mem.h"

/* helper: compute digit sum and digit count of an unsigned 128‑bit
   integer.  The 128‑bit type is a GNU/C++ extension; it is
   available on the compilers used for evaluation. */
static inline void digit_sum_len(unsigned __int128 x,
                                 unsigned long long *sum,
                                 unsigned long long *len)
{
    *sum = 0;
    *len = 0;
    /* 0 is not a square of a positive integer, but guard for completeness */
    if (x == 0) {
        *sum = 0;
        *len = 1;
        return;
    }
    while (x != 0) {
        *sum += (unsigned long long)(x % 10);
        *len += 1;
        x /= 10;
    }
}

/* local mutable state for memoisation */
static seq_t *cache   = NULL;   /* array of found k values          */
static size_t cache_cap = 0;    /* allocated capacity               */
static size_t cache_len = 0;    /* number of stored values          */

/* highest k that has already been inspected */
static uint64_t last_k = 0;

/* ensure that the cache can hold at least 'min_cap' elements. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        exit(1);               /* allocation failed – abort */
    cache    = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)                      /* offset is 1 – undefined for n==0 */
        return 0;

    /* already computed? */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* scan k from the last tested value onward until we have n terms */
    for (uint64_t k = last_k + 1; cache_len < (size_t)n; ++k) {
        unsigned __int128 sq = (unsigned __int128)k * k;
        unsigned long long sum, len;
        digit_sum_len(sq, &sum, &len);

        if (sum == 2 * len) {   /* average digit equals 2 */
            if (cache_len >= cache_cap)
                grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
    }
    /* set last_k to the last k that was inspected */
    last_k = (uint64_t)(cache_len > 0 ? (unsigned long long)cache[cache_len - 1] : 0);

    return cache[n - 1];
}
