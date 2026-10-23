/*
 * A129630  Numbers k such that sum of digits of (k+1) is a prime.
 * https://oeis.org/A129630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A028834(n) – 1, where A028834(n) is the n-th integer
 *            whose decimal digit sum is a prime number. The sequence is
 *            defined for n ≥ 1
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   digit_sum, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_small.h"

/* Cache for already generated terms.  The cache grows lazily as
   larger indices are requested. */
static seq_t *cache          = NULL;
static size_t cache_size     = 0;   /* number of terms currently stored   */
static size_t cache_capacity = 0;   /* allocated size of cache array       */

/* m is the next candidate integer to test.  Sequence terms are
   generated from m = 2 upward. */
static idx_t next_m = 2;

/* Helper to enlarge the cache when necessary. */
static void ensure_cache_size(size_t needed)
{
    if (cache_capacity >= needed)
        return;
    size_t new_cap = cache_capacity == 0 ? 16 : cache_capacity * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* If realloc fails, abort – the build environment expects
           a usable implementation. */
        abort();
    }
    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1; undefined for n = 0 */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        seq_t m_seq = (seq_t)next_m;
        seq_t dsum  = digit_sum(m_seq);
        if (is_prime_small(dsum)) {
            seq_t val = m_seq - 1;   /* a(n) = A028834(n) – 1 */
            ensure_cache_size(cache_size + 1);
            cache[cache_size++] = val;
        }
        ++next_m;
    }

    return cache[(size_t)n - 1];
}
