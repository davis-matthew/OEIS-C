/*
 * A049856  a(n) = (Sum{k=0..n-1} a(k)) - a(n-3), with a(0)=0, a(1)=0, a(2)=1.
 * https://oeis.org/A049856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0}^{n-1} a(k) - a(n-3) with a(0)=0, a(1)=0, a(2)=1
 *            Recurrence used for n≥4: a(n)=2*a(n-1)-a(n-3)+a(n-4)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 38, u64 n <= 74, u128 n <= 145,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    /* handle very small n directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* first time: allocate base values up to index 2 */
    if (!cache) {
        /* allocate at least 4 slots to hold a(0)..a(3) */
        ensure_cache(3, &cache, &cache_cap);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        max_computed = 2;
    }

    /* grow cache if needed */
    if ((size_t)n >= cache_cap)
        ensure_cache(n, &cache, &cache_cap);

    /* compute missing terms only once */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 3) {
                /* a(3) = a(0)+a(1)+a(2)-a(0) = 1 */
                cache[i] = (seq_t)1;
            } else {
                /* a(i) = 2*a(i-1) - a(i-3) + a(i-4) */
                seq_t temp = cache[i-1] * (seq_t)2 + cache[i-4];
                temp -= cache[i-3];
                cache[i] = temp;
            }
        }
        max_computed = n;
    }

    return cache[n];
}
