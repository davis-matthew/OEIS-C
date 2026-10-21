/*
 * A097162  a(n) = Sum_{k=0..n} C(floor((n+1)/2),floor((k+1)/2))*2^k.
 * https://oeis.org/A097162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} C(floor((n+1)/2),floor((k+1)/2))*2^k.
 *            Recurrence: a(n+5)=20*a(n)-20*a(n+1)-9*a(n+2)+9*a(n+3)+a(n+4).
 *            Base values: a(0)=1, a(1)=3, a(2)=7, a(3)=21, a(4)=37, a(5)=123,
 *            a(6)=187
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 54, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* cached values */
static size_t cache_len = 0;    /* number of stored terms */
static size_t cache_cap = 0;    /* allocated capacity */

/* initialise first seven terms (indices 0..6) */
static void init_base(void)
{
    seq_t init7[7] = {(seq_t)1, (seq_t)3, (seq_t)7, (seq_t)21, (seq_t)37, (seq_t)123, (seq_t)187};

    cache_cap = 16;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    for (size_t i = 0; i < 7; ++i)
        cache[i] = init7[i];
    cache_len = 7;
}

/* ensure cache contains values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_len > n)           /* already computed */
        return;

    if (cache_len == 0)                 /* first time */
        init_base();

    /* grow capacity if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t newcap = cache_cap;
        while (newcap < (size_t)(n + 1))
            newcap *= 2;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* recurrence is valid only for i >= 7 */
        if (i < 7) {
            /* indices 0..6 are already initialized */
            continue;
        }
        seq_t pos = (seq_t)20 * cache[i - 5] + (seq_t)9 * cache[i - 2] + cache[i - 1];
        seq_t neg = (seq_t)20 * cache[i - 4] + (seq_t)9 * cache[i - 3];
        cache[i] = pos - neg;          /* pos >= neg for all i >= 7 */
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
