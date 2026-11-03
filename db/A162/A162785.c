/*
 * A162785  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=15, a(2)=210, a(3)=2835;
 *            a(n)=13*a(n-1)+13*a(n-2)-91*a(n-3) for n≥4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 643 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* allocate or grow the cache, compute new terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Grow the cache to contain indices up to n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_len);
        cache = tmp;

        /* Compute all new entries */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)15;
            } else if (i == 2) {
                cache[i] = (seq_t)210;
            } else if (i == 3) {
                cache[i] = (seq_t)2835;
            } else {
                seq_t t1 = mul_small(cache[i - 1], 13U);
                seq_t t2 = mul_small(cache[i - 2], 13U);
                seq_t t3 = mul_small(cache[i - 3], 91U);
                cache[i] = t1 + t2 - t3;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
