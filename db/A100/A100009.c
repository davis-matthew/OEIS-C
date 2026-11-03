/*
 * A100009  a(1) = 2; for n >= 2, a(n) = f(a(n-1)) where f(n) =
 *          n*(n+1)*(n+2)*(n+3)/24.
 * https://oeis.org/A100009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(1)=2; for n>=2,
 *            a(n)=a(n-1)*(a(n-1)+1)*(a(n-1)+2)*(a(n-1)+3)/24
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap  = 0;

static void ensure_capacity(idx_t min_cap) {
    if (min_cap <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < min_cap) new_cap *= 2;
    cache = seq_realloc(cache, new_cap);
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */
    if (!cache) {                         /* initialize cache */
        cache_cap  = 1;
        cache      = seq_malloc(cache_cap);
        cache[0]   = (seq_t)2;            /* a(1) */
        cache_size = 1;
    }
    while (cache_size < n) {
        seq_t x    = cache[cache_size - 1];
        seq_t next = x * (x + 1);
        next       = next * (x + 2);
        next       = next * (x + 3);
        next       = next / 24;
        ensure_capacity(cache_size + 1);
        cache[cache_size++] = next;
    }
    return cache[n - 1];
}
