/*
 * A050045  a(n) = a(n-1) + a(m) for n >= 4, where m = 2^(p+1) + 2 - n and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1 and a(2) = a(3) = 2.
 * https://oeis.org/A050045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 74, u32 n <= 516, u64 n <= 4391,
 *            u128 n <= 8193, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A050045
 *  a(1) = 1, a(2) = 2, a(3) = 2
 *  For n ≥ 4 : a(n) = a(n - 1) + a(m)
 *      where m = 2^(p+1) + 2 - n
 *      and p is the integer with  2^p < n - 1 ≤ 2^(p+1)
 *
 * This implementation memoises already‑computed values in a static
 * dynamic array so that calls in arbitrary order are efficient.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Sanity: n must be ≥ 1 according to the OEIS definition */
    if (n < 1)
        return (seq_t)0;          /* Undefined value for indexes < 1 */

    /* Static cache: 1‑based indexing; cache[0] is unused */
    static seq_t *cache = NULL;           /* Pointer to the array */
    static idx_t cache_cap = 0;           /* Allocated capacity (max index stored) */
    static idx_t cache_size = 0;          /* Highest index that has a defined value */

    /* Resize the cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* Allocation failure: return 0 as a safe sentinel */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base values if not already initialised */
    if (cache_size < 3) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)2;
        cache_size = 3;
    }

    /* Compute the missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* For i ≥ 4, find p such that 2^p < i-1 ≤ 2^(p+1) */
        unsigned p = floor_log2_uint64(i - 2);   /* floor(log2(i-2)) gives p */
        idx_t power = ((idx_t)1) << (p + 1);     /* 2^(p+1) */
        idx_t m = power + 2 - i;                 /* m < i for i ≥ 4 */
        cache[i] = cache[i - 1] + cache[m];
    }
    cache_size = n;

    return cache[n];
}
