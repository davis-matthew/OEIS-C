/*
 * A235915  a(1) = 1, a(n) = a(n-1) + (digsum(a(n-1)) mod 5) + 1, digsum =
 *          A007953.
 * https://oeis.org/A235915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(n) = a(n-1) + (digsum(a(n-1)) mod 5) + 1 */

#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/seq_alloc.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1 */

    /* Ensure the cache holds at least n elements */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = seq_alloc(new_cap);
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        cache = new_cache;
        cache_cap = new_cap;
    }

    if (n <= max_computed)
        return cache[n - 1];

    seq_t prev;
    if (max_computed == 0) {          /* first term */
        prev = (seq_t)1;
        cache[0] = prev;
        max_computed = 1;
    } else {
        prev = cache[max_computed - 1];
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = digit_sum(prev);
        seq_t inc = (sum % (seq_t)5) + (seq_t)1;
        seq_t next = prev + inc;
        cache[i - 1] = next;
        prev = next;
    }
    max_computed = n;
    return cache[n - 1];
}
