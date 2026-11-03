/*
 * A077827  Expansion of (1-x)^(-1)/(1-2*x-2*x^2-2*x^3).
 * https://oeis.org/A077827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - 2*a(n-4)
   with initial values a(0)=1, a(1)=3, a(2)=9, a(3)=27 */
#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static size_t   last  = 0;      /* largest index computed */

    size_t target = (size_t)n;

    /* Initial allocation: store a(0)=1 */
    if (cache == NULL) {
        cap   = 1;
        cache = mem_new(seq_t, cap);
        if (!cache) abort();
        cache[0] = (seq_t)1;
        last = 0;
    }

    /* Grow array if necessary */
    if (target + 1 > cap) {
        size_t new_cap = target + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms */
    if (target > last) {
        for (size_t i = last + 1; i <= target; ++i) {
            if (i <= 3) {
                /* a(i) = 3*a(i-1) for i=1..3 */
                cache[i] = mul_small(cache[i - 1], 3);
            } else {
                seq_t term1 = mul_small(cache[i - 1], 3);
                seq_t term2 = mul_small(cache[i - 4], 2);
                cache[i] = term1 - term2;
            }
        }
        last = target;
    }

    return cache[target];
}
