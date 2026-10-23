/*
 * A124370  Expansion of 1/(1-2x^2-3x^3-x^4).
 * https://oeis.org/A124370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-2) + 3*a(n-3) + a(n-4) with initial values a(0)=1,
 *            a(1)=0, a(2)=2, a(3)=3
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding computed terms          */
static size_t cache_size = 0;    /* current allocated size of cache       */
static size_t computed_up_to = 0;/* next index that is not yet computed   */

seq_t a(idx_t n)
{
    size_t needed = (size_t)n + 1;

    /* grow cache if necessary */
    if (needed > cache_size) {
        seq_t *new_cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) abort();     /* allocation failed */
        cache = new_cache;
        cache_size = needed;
    }

    /* compute missing terms up to n */
    if (computed_up_to < needed) {
        for (size_t i = computed_up_to; i < needed; ++i) {
            if (i == 0)          cache[i] = (seq_t)1;
            else if (i == 1)     cache[i] = (seq_t)0;
            else if (i == 2)     cache[i] = (seq_t)2;
            else if (i == 3)     cache[i] = (seq_t)3;
            else {
                seq_t val = (seq_t)0;
                val += cache[i - 2] * (seq_t)2;
                val += cache[i - 3] * (seq_t)3;
                val += cache[i - 4];
                cache[i] = val;
            }
        }
        computed_up_to = needed;
    }

    return cache[n];
}
