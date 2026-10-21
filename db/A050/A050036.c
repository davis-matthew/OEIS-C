/*
 * A050036  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 3 - 2^(p+1) and
 *          2^p < n - 1 <= 2^(p+1), starting with a(1) = a(2) = 1 and a(3) =
 *          4.
 * https://oeis.org/A050036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 193, u64 n <= 768,
 *            u128 n <= 3618, big any n
 * Helpers:   alloc_seq_array, ensure_cache, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
 *   a(1) = 1,  a(2) = 1,  a(3) = 4
 *   For n ≥ 4:
 *       let p be the unique integer with 2^p < n-1 ≤ 2^(p+1)
 *       m = 2*n - 3 - 2^(p+1)
 *       a(n) = a(n-1) + a(m) 
 * All indices are 1‑based.  The implementation memoises the
 * computed values in a growing array so that any order of calls
 * is supported.  The array is allocated via the helper
 * `alloc_seq_array` or `ensure_cache` (see below).  The helper
 * `floor_log2_uint64` is used to obtain the exponent p. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"      /* if needed for allocation */
#include "../utils/ensure_cache.h"
#include "../utils/floor_log2_uint64.h"

static seq_t *cache           = NULL; /* 1‑based: cache[0] unused */
static size_t cache_capacity  = 0;      /* number of elements allocated */
static idx_t  cache_last      = 0;      /* largest index that has been computed */

/* Grow or allocate the cache so that it holds indices up to n (inclusive). */
static void
ensure_cache_up_to(idx_t n)
{
    /* Ensure capacity is at least n+1 so that index n is addressable. */
    ensure_cache(n + 1, &cache, &cache_capacity);
}

/* Compute values up to n and update cache_last. */
static void
compute_to(idx_t n)
{
    if (n <= cache_last)
        return;

    ensure_cache_up_to(n);

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i == 1 || i == 2) {
            cache[i] = (seq_t)1;
        } else if (i == 3) {
            cache[i] = (seq_t)4;
        } else {
            /* p satisfies 2^p < i-1 ≤ 2^(p+1)  →  p = floor_log2_uint64(i-2) */
            idx_t p = floor_log2_uint64(i - 2);
            idx_t two_pow = (idx_t)1 << (p + 1);           /* 2^(p+1) */
            idx_t m = 2 * i - 3 - two_pow;                 /* 1 ≤ m < i */
            cache[i] = cache[i - 1] + cache[m];
        }
    }
    cache_last = n;
}

/* Public sequence function.  The function is safe for any call order
 * and for all n ≥ 1.  For n < 1 the function returns 0. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    compute_to(n);
    return cache[n];
}
