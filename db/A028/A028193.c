/*
 * A028193  Expansion of 1/((1-5*x)*(1-8*x)*(1-10*x)*(1-11*x)).
 * https://oeis.org/A028193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   mul_small, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1,  a(1) = 34,
   a(n) = 21*a(n-1) - 110*a(n-2) + (8^(n+1)-5^(n+1))/3   for n≥2  */
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Cached sequence values and auxiliary data */
    static seq_t *cache      = NULL;
    static idx_t cache_cap   = 0;   /* allocated capacity */
    static idx_t cache_len   = 0;   /* number of initialized terms  */
    static seq_t pow8_cur, pow5_cur;  /* 8^(k+1) and 5^(k+1) for current k */

    /* First call: allocate and initialise a(0) and the power registers. */
    if (cache == NULL) {
        size_t cap = (size_t)(n + 1 > 2 ? n + 1 : 2);
        cache    = seq_alloc(cap);
        cache_cap = (idx_t)cap;
        cache[0] = (seq_t)1;
        cache_len = 1;
        pow8_cur = (seq_t)8;   /* 8^1 */
        pow5_cur = (seq_t)5;   /* 5^1 */
    }
    /* Ensure a(1) is present if needed. */
    if (cache_len == 1 && n >= 1) {
        cache[1] = (seq_t)34;
        cache_len = 2;
        pow8_cur = mul_small(pow8_cur, 8);   /* now 8^2 */
        pow5_cur = mul_small(pow5_cur, 5);   /* now 5^2 */
    }
    /* If the requested index is already cached, return it. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache, extending it up to the requested index. */
    while (cache_len <= n) {
        /* Grow the underlying array if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap == 0 ? 2 : cache_cap << 1;
            while (new_cap <= n) new_cap <<= 1;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = (idx_t)new_cap;
        }

        /* Update the powers for the next index. */
        pow8_cur = mul_small(pow8_cur, 8);   /* 8^(k+1) */
        pow5_cur = mul_small(pow5_cur, 5);   /* 5^(k+1) */

        /* (8^(k+1) - 5^(k+1)) / 3  */
        seq_t diff = pow8_cur - pow5_cur;
        seq_t term = diff / (seq_t)3;

        /* 21*a(k-1) - 110*a(k-2) + term  */
        seq_t t1 = mul_small(cache[cache_len-1], 21);
        seq_t t2 = mul_small(cache[cache_len-2], 110);
        seq_t val = t1 + term;
        val -= t2;

        cache[cache_len] = val;
        cache_len++;
    }

    return cache[n];
}
