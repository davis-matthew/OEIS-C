/*
 * A052602  E.g.f. x^2*(1-x)/(1-x-x^2).
 * https://oeis.org/A052602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=0, a(1)=0, a(2)=2, a(3)=0, 
 * for n ≥ 4: a(n) = (n-1)*n*a(n-2) + n*a(n-1)
 *    (equivalent to the EGF x^2*(1-x)/(1-x-x^2))
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache      = NULL;  /* memoised terms */
static idx_t  cache_len  = 0;     /* number of valid terms in cache */
static size_t cache_cap  = 0;     /* capacity of cache (in terms) */

/* ensure cache can hold at least indices 0 … n */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > (size_t)n) return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* out of memory */
    cache      = tmp;
    cache_cap  = new_cap;
    if (cache_len == 0) {              /* initialise the known terms */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 2;
        cache[3] = 0;
        cache_len = 4;
    }
}

seq_t a(idx_t n)
{
    if (cache_len == 0) ensure_capacity(3);

    /* already known */
    if (n < cache_len) return cache[n];

    ensure_capacity(n);

    for (idx_t k = cache_len; k <= n; ++k) {
        if (k < 4) continue;               /* base terms already set */
        /* recurrence: a(k) = (k-1)*k*a(k-2) + k*a(k-1) */
        seq_t mul1 = (seq_t)(k * (k - 1));
        seq_t mul2 = (seq_t)k;
        seq_t term1 = mul1 * cache[k - 2];
        seq_t term2 = mul2 * cache[k - 1];
        cache[k] = term1 + term2;
    }
    cache_len = n + 1;
    return cache[n];
}
