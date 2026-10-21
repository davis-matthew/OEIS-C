/*
 * A050046  a(n) = a(n-1) + a(m) for n >= 4, where m = n - 1 - 2^p and p is
 *          the unique integer such that 2^p < n - 1 <= 2^(p+1), starting with
 *          a(1) = 1 and a(2) = a(3) = 2.
 * https://oeis.org/A050046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(m) where m = n-1-2^p and 2^p < n-1 <= 2^(p+1);
 *            initial values: a(1)=1, a(2)=2, a(3)=2
 * Range:     u8 n <= 32, u16 n <= 200, u32 n <= 1755, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

static inline idx_t floor_log2_idx(idx_t x)
{
    idx_t p = 0;
    while (x > 1) {
        x >>= 1;
        ++p;
    }
    return p;
}

static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = (size_t)(n + 1);          /* allow indices up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;

    if (cache_cap == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)2;
    }

    for (idx_t i = cache_cap + 1; i <= n; ++i) {
        if (i < 4) continue;                 /* already initialised */
        seq_t prev = cache[i - 1];
        idx_t p = floor_log2_idx(i - 2);     /* since 2^p < i-1 <= 2^(p+1) */
        idx_t power = (idx_t)1 << p;
        idx_t m = (i - 1) - power;           /* m < i */
        prev += cache[m];
        cache[i] = prev;
    }
    cache_cap = n;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)2;
    ensure_cache(n);
    return cache[n];
}
