/*
 * A086615  Antidiagonal sums of triangle A086614.
 * https://oeis.org/A086615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, for n≥2 a(n) = (n+1) + Σ_{k=0}^{n-2} a(k) *
 *            a(n-2-k) (uses the recurrence derived from A(x)=1/(1−x)^2 +
 *            x^2*A(x)^2)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* holds computed a(n) */
static size_t cached_len = 0;     /* number of valid entries in cache */
static size_t cache_cap  = 0;     /* allocated capacity of cache */

/* enlarge cache so that it can hold at least new_len elements */
static void ensure_cache_cap(size_t new_len)
{
    if (new_len <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 1 : cache_cap << 1;
    while (new_cap < new_len) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* extend cache up to index n */
    if (n >= cached_len) {
        size_t next_len = n + 1;
        ensure_cache_cap(next_len);
        for (size_t i = cached_len; i < next_len; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)2;
            } else {
                seq_t sum = 0;
                /* Σ_{k=0}^{i-2} cache[k] * cache[i-2-k] */
                for (size_t k = 0; k <= i-2; ++k) {
                    sum += cache[k] * cache[i-2-k];
                }
                cache[i] = (seq_t)i + (seq_t)1 + sum;
            }
        }
        cached_len = next_len;
    }
    return cache[n];
}
