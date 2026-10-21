/*
 * A097119  Expansion of (1-x)^2/((1-x)^3-2x^4).
 * https://oeis.org/A097119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + 2*a(n-4), with a(0) = a(1)
 *            = a(2) = a(3) = 1, a(4) = 3
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values of a(n) */
static size_t cache_len = 0;     /* allocated size of cache */
static size_t computed_len = 0;  /* number of terms already computed */

/* Allocate or enlarge the cache so that it can hold indices up to n.
   Newly allocated elements are left zero, they will be filled on demand. */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache_len >= need) return;

    size_t new_len = cache_len > 0 ? cache_len : 16;
    while (new_len < need) new_len <<= 1;    /* grow geometrically */

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */
    cache = tmp;
    cache_len = new_len;
}

/* Compute a(n) for any n>=0, using memoisation and the recurrence. */
seq_t a(idx_t n)
{
    if (n + 1 > computed_len) {
        size_t target = (size_t)n + 1;
        ensure_cache((idx_t)target);

        for (idx_t i = (idx_t)computed_len; i <= n; ++i) {
            if (i == 0 || i == 1 || i == 2 || i == 3) {
                cache[i] = (seq_t)1;
            } else if (i == 4) {
                cache[i] = (seq_t)3;
            } else {
                cache[i] = (seq_t)3 * cache[i - 1]
                         - (seq_t)3 * cache[i - 2]
                         + cache[i - 3]
                         + (seq_t)2 * cache[i - 4];
            }
        }
        computed_len = n + 1;
    }
    return cache[n];
}
