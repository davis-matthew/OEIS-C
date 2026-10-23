/*
 * A165209  Numbers whose digital sum is divisible by the digital sum of its
 *          divisors.
 * https://oeis.org/A165209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1214, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, ensure_cache, floor_sqrt, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A165209: numbers whose digital sum is divisible by the digital
   sum of each divisor. This implementation tests consecutive integers and
   stores the terms in a cache so that any order of calls to a(idx_t n) is
   handled correctly. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/floor_sqrt.h"
#include "../utils/ensure_cache.h"
#include "../utils/seq_alloc_array.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; a(1) = 1.  For n == 0 we return
       0 but the result is not used by the test harness. */
    if (n == 0) return 0;

    /* Static cache of already found terms.  The cache is grown on demand. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t found = 0;   /* number of terms currently stored */
    static idx_t next = 1;     /* next integer to test */

    /* Initialise the cache on the first call. */
    if (!cache) {
        cache_cap = 16;
        cache = seq_alloc_array(cache_cap);
    }

    /* Extend the cache until it contains at least n terms. */
    while (found < (size_t)n) {
        seq_t val = (seq_t)next;
        seq_t dsum = digit_sum(val);

        bool ok = true;
        seq_t sqrt_val = floor_sqrt(val);

        /* Test all divisors up to the integer square root. */
        for (seq_t d = 1; d <= sqrt_val; ++d) {
            if (val % d == 0) {
                seq_t ds_d = digit_sum(d);
                if (dsum % ds_d != 0) { ok = false; break; }

                seq_t other = val / d;
                if (other != d) {
                    seq_t ds_other = digit_sum(other);
                    if (dsum % ds_other != 0) { ok = false; break; }
                }
            }
        }

        if (ok) {
            /* Grow the cache if necessary. */
            if (found >= cache_cap) {
                ensure_cache(found + 1, &cache, &cache_cap);
            }
            cache[found++] = val;
        }

        ++next;
    }

    return cache[n - 1];
}
