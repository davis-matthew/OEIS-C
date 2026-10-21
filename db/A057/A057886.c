/*
 * A057886  Number of integer 4-tuples that give the lengths of the sides of a
 *          nondegenerate quadrilateral with perimeter n.
 * https://oeis.org/A057886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + d(n-1) (n>1) where d(m) =
 *            floor(m/4)*floor((m-2)/4) if m even, d(m) = floor((m+1)/4) if m
 *            odd. a(1) = 0. This recurrence reproduces the OEIS A057886 terms
 * Range:     u8 n <= 29, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   d_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/d_of.h"

static seq_t *cache = NULL;   /* stores a[1..] with a[0] unused */
static idx_t  cache_len = 0;  /* highest n already computed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not defined, but safe */

    /* Initialize cache for a[1] */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)0;                 /* unused slot */
        cache[1] = (seq_t)0;                 /* a(1) = 0 */
        cache_len = 1;
    }

    /* Extend cache if needed */
    if (n > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;     /* realloc failed */
        cache = new_cache;
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            seq_t prev = cache[k - 1];
            seq_t add  = d_of(k - 1);
            cache[k] = prev + add;
        }
        cache_len = n;
    }

    return cache[n];
}
