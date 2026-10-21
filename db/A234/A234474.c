/*
 * A234474  Numbers that are divisible by their digital sum and digital root.
 * https://oeis.org/A234474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer divisible by both its digital
 *            sum and its digital root (in base 10)
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the helper that computes the digital sum of a value. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

#ifdef __cplusplus
#  include <vector>
#endif

seq_t a(idx_t n)
{
    /* Return the cached value if already found. */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
#else
    static seq_t  *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
#endif
    static idx_t last_checked = 0;   /* last integer _tested_ */

    if (n == 0) return 0; /* sequence is defined for n>=1 */

    /* Fast path: return from cache when possible. */
#ifdef __cplusplus
    if (cache.size() >= n) return cache[n-1];
#else
    if (cache_size >= n) return cache[n-1];
#endif

    /* Ensure we keep searching until we have at least n terms. */
    while (true)
    {
        /* Move to the next candidate integer. */
        last_checked++;

        /* Convert the candidate into seq_t. */
        seq_t val = (seq_t)last_checked;

        /* Compute the digital sum. */
        seq_t sum = digit_sum(val);
        if (sum == 0) continue;                         /* impossible for val>=1 */

        /* Quick test: divisor of its digital sum. */
        if (val % sum != 0) continue;

        /* Compute the digital root by repeatedly summing digits. */
        seq_t root = sum;
        while (root >= 10) root = digit_sum(root);

        /* Final test: divisor of its digital root. */
        if (val % root != 0) continue;

        /* Store the found value. */
#ifdef __cplusplus
        cache.push_back(val);
#else
        /* Grow the C buffer if needed. */
        if (cache_size == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
#ifdef __cplusplus
            seq_t *tmp = static_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size++] = val;
#endif
        /* Check if we now have enough terms. */
#ifdef __cplusplus
        if (cache.size() >= n) return cache[n-1];
#else
        if (cache_size >= n)  return cache[n-1];
#endif
    }
}
