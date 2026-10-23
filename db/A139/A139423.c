/*
 * A139423  a(1)=1, a(2)=2. For n >= 3, a(n) = a(n-1) + d(a(n-1)) + d(a(n-2)),
 *          where d(m) is the number of positive divisors of m.
 * https://oeis.org/A139423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + d(a(n-1)) + d(a(n-2)), with a(1) = 1, a(2) = 2
 *            and d(m) = number of positive divisors of m
 * Range:     u8 n <= 39, u16 n <= 5093, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/divisor_count_u64.h"

static seq_t *cache = NULL;
static size_t capacity = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    if (n < 1)            /* below offset – undefined, but harmless */
        return (seq_t)0;

    /* Ensure space for at least two terms. */
    ensure_cache(2, &cache, &capacity);

    /* Initialise the first two terms on first use. */
    if (max_computed == 0) {
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        max_computed = 2;
    }

    /* Resize cache if a larger index was requested. */
    if ((size_t)n > capacity)
        ensure_cache(n, &cache, &capacity);

    /* Compute all missing terms up to n. */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* a(i-1) is stored at index i-2, a(i-2) at i-3. */
            seq_t prev     = cache[i - 2];
            seq_t prevprev = cache[i - 3];
            uint64_t d_prev   = divisor_count_u64((uint64_t)prev);
            uint64_t d_pprev  = divisor_count_u64((uint64_t)prevprev);
            cache[i - 1] = prev + (seq_t)d_prev + (seq_t)d_pprev;
        }
        max_computed = n;
    }

    return cache[n - 1];
}
