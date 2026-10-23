/*
 * A188128  Expansion of (4-6*x-6*x^2+x^3)/((1+x)*(1-3*x+x^3)).
 * https://oeis.org/A188128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+3*a(n-2)-a(n-3)-a(n-4), with a(0)=4, a(1)=2,
 *            a(2)=10, a(3)=23
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static idx_t  cache_cap = 0;

static void
init_cache(void)
{
    cache_cap = 4;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (!cache) return;          /* allocation failure: return garbage later */
    cache[0] = (seq_t)4;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)10;
    cache[3] = (seq_t)23;
    cache_len = 4;
}

static void
ensure_cache_size(idx_t needed)
{
    if (needed < cache_cap) return;
    idx_t new_cap = cache_cap == 0 ? 4 : cache_cap;
    while (needed >= new_cap) new_cap <<= 1;   /* double until big enough */
    seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) return;                     /* allocation failure: stay old */
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t
a(idx_t n)
{
    if (!cache) init_cache();
    if (!cache) return (seq_t)0;  /* if init failed */

    if (n < cache_len) return cache[n];

    ensure_cache_size(n + 1);
    if (!cache) return (seq_t)0;  /* if realloc failed */

    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i-1]
                 + (seq_t)3 * cache[i-2]
                 - cache[i-3]
                 - cache[i-4];
    }
    cache_len = n + 1;
    return cache[n];
}
