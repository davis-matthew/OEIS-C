/*
 * A175723  a(1)=a(2)=1; thereafter a(n) = gpf(a(n-1)+a(n-2)), where gpf =
 *          "greatest prime factor".
 * https://oeis.org/A175723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 1; for n≥3, a(n) = gpf(a(n-1)+a(n-2))
 * Range:     u8 n <= 121, u16 n <= 121, u32 n <= 121, u64 n <= 121,
 *            u128 n <= 121, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gpf.h"

static seq_t *cache = NULL;              /* cache[1] holds a(1) */
static size_t cache_cap = 0;            /* allocated capacity (index 0 unused) */
static size_t computed_max = 0;         /* largest index for which a(i) is computed */

/* Grow the cache to accommodate indices up to ‘needed’ (inclusive). */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = needed + 128;      /* reasonable growth factor */
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n) for any n ≥ 1.  Calls are independent of order. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n<1, return 0 */
    size_t target = (size_t)n;

    if (computed_max < target) {
        grow_cache(target);
        for (size_t i = computed_max + 1; i <= target; ++i) {
            seq_t val;
            if (i == 1 || i == 2) {
                val = (seq_t)1;
            } else {
                uint64_t sum = (uint64_t)cache[i-1] + (uint64_t)cache[i-2];
                uint64_t g = gpf(sum);
                val = (seq_t)g;
            }
            cache[i] = val;
        }
        computed_max = target;
    }
    return cache[target];
}
