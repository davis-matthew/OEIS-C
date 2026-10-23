/*
 * A180678  The Ze2 sums of the Pell-Jacobsthal triangle A013609.
 * https://oeis.org/A180678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 8*a(n-3), a(0)=1, a(1)=2, a(2)=5
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    /* Initialize cache with the first three terms if needed */
    if (cache_len == 0) {
        cache = alloc_seq_array(3);
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 5;
        cache_len = 3;
    }

    /* If we already have terms up to n, nothing to do */
    if (n < cache_len) return;

    /* Allocate a larger buffer and copy already computed terms */
    seq_t *new_cache = alloc_seq_array(n + 1);
    for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];

    /* Compute the remaining terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Compute 6*a(i-1) + 8*a(i-3) first to avoid underflow */
        seq_t val = 6 * new_cache[i - 1] + 8 * new_cache[i - 3];
        val = val - 11 * new_cache[i - 2];
        new_cache[i] = val;
    }

    /* Replace old cache with the expanded one */
    cache = new_cache;
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
