/*
 * A073121  a(n) = r*a(ceiling(n/2)) + s*a(floor(n/2)) with a(1)=1 and
 *          (r,s)=(2,2).
 * https://oeis.org/A073121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^c · (n + 2b) where n = 2^c + b, 0 ≤ b < 2^c.
 *            Recurrence: a(1) = 1, a(n) = 2·(a(⌊n/2⌋) + a(⌈n/2⌉)) for n ≥ 2
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Fast floor(log2) for 64‑bit unsigned integers. */
static inline idx_t floor_log2_u64(idx_t x)
{
    /* x ≥ 1 by assumption. */
    return 63 - __builtin_clzll((unsigned long long)x);
}

/* Static cache of previously computed values. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of allocated slots, 0..cache_len-1  */

/* Compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* out‑of‑range request, not used in tests */

    /* Grow the cache if necessary. */
    if (n >= cache_len)
    {
        idx_t new_len = n + 1;           /* we need indices 0..n */
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* Compute all new entries. */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 0)        /* unused slot, keep as 0 */
            {
                cache[i] = (seq_t)0;
                continue;
            }
            /* Compute a(i) using the closed form. */
            idx_t c   = floor_log2_u64(i);
            idx_t b   = i - ((idx_t)1 << c);
            seq_t p2  = ((seq_t)1) << c;                 /* 2^c */
            seq_t val = p2 * ((seq_t)i + ((seq_t)2) * (seq_t)b);
            cache[i] = val;
        }
        cache_len = new_len;
    }
    return cache[n];
}
