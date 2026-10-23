/*
 * A143452  Expansion of 1/(x^k*(1-x-2*x^(k+1))) for k=9.
 * https://oeis.org/A143452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(n)=2*n+1 for 0<=n<=10; a(n)=a(n-1)+2*a(n-10) for n>=11
 * Range:     u8 n <= 19, u16 n <= 44, u32 n <= 92, u64 n <= 189,
 *            u128 n <= 384, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t last = (idx_t)(-1);

/* Ensure the cache can hold indices up to n. */
static void ensure_capacity(idx_t n)
{
    if (n < cap) return;
    size_t old_cap = cap;
    size_t new_cap = old_cap ? old_cap * 2 : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    for (size_t i = 0; i < old_cap; ++i) new_cache[i] = cache[i];
    if (cache) free(cache);
    cache = new_cache;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    if (last == (idx_t)(-1)) {            /* first call: compute from 0 .. n */
        for (idx_t i = 0; i <= n; ++i) {
            if (i <= 10)
                cache[i] = ((seq_t)i * 2) + 1;
            else
                cache[i] = cache[i-1] + ((seq_t)2 * cache[i-10]);
        }
        last = n;
        return cache[n];
    }
    if (n <= last)                         /* already computed */
        return cache[n];
    for (idx_t i = last + 1; i <= n; ++i) {/* compute missing entries */
        if (i <= 10)
            cache[i] = ((seq_t)i * 2) + 1;
        else
            cache[i] = cache[i-1] + ((seq_t)2 * cache[i-10]);
    }
    last = n;
    return cache[n];
}
