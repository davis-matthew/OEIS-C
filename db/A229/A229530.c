/*
 * A229530  Number of defective 3-colorings of an n X 4 0..2 array connected
 *          horizontally, diagonally and antidiagonally with exactly one
 *          mistake, and colors introduced in row-major 0..2 order.
 * https://oeis.org/A229530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 57*a(n-2) + 56*a(n-3) - 16*a(n-4) (for n > 5)
 *            a(1) = 6, a(2) = 36, a(3) = 361, a(4) = 3064, a(5) = 24344
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   ensure_cache, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n == 0 we return 0. */
    if (n == 0)
        return 0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0; /* largest n for which cache[n] is valid */

    /* Prepare cache capacity */
    ensure_cache(n, &cache, &cap);

    /* If already computed, return the cached value */
    if (n <= max_computed)
        return cache[n];

    /* Compute values up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t val;
        if (k == 1) {
            val = 6;
        } else if (k == 2) {
            val = 36;
        } else if (k == 3) {
            val = 361;
        } else if (k == 4) {
            val = 3064;
        } else if (k == 5) {
            val = 24344;
        } else {
            /* Recurrence: a(k) = 14*a(k-1) - 57*a(k-2) + 56*a(k-3) - 16*a(k-4) */
            seq_t t1 = cache[k-1] * 14;      /* 14*a(k-1) */
            seq_t t2 = cache[k-2] * 57;      /* 57*a(k-2) */
            seq_t t3 = cache[k-3] * 56;      /* 56*a(k-3) */
            seq_t t4 = cache[k-4] * 16;      /* 16*a(k-4) */

            /* Compute (14*a(k-1) + 56*a(k-3)) - (57*a(k-2) + 16*a(k-4)) */
            seq_t sum1 = t1 + t3;
            seq_t sum2 = t2 + t4;

            /* sum1 is always ≥ sum2 for n ≥ 6, so subtraction is safe. */
            val = sum1 - sum2;
        }
        cache[k] = val;
    }

    max_computed = n;
    return cache[n];
}
