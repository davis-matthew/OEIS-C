/*
 * A042197  Denominators of continued fraction convergents to sqrt(623).
 * https://oeis.org/A042197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 1248*a(n-4) - a(n-8) (n > 7) Initial values: a(0)=1,
 *            a(1)=1, a(2)=24, a(3)=25, a(4)=1224, a(5)=1249, a(6)=29951,
 *            a(7)=31200
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 199 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

seq_t a(idx_t n)
{
    /* static cache for values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t computed = 0;          /* highest index computed so far */

    /* initialise cache with initial terms */
    if (computed == 0) {
        cap = 16;                         /* initial capacity */
        cache = mem_new(seq_t, cap);
        if (!cache) {                     /* allocation failed */
            return (seq_t)0;
        }
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 24;
        cache[3] = 25;
        cache[4] = 1224;
        cache[5] = 1249;
        cache[6] = 29951;
        cache[7] = 31200;
        computed = 7;
    }

    /* extend cache capacity if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) {
            new_cap *= 2;
        }
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {                          /* reallocation failed */
            return (seq_t)0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* use mul_small to avoid overflow for 64‑bit builds: */
        seq_t prod = mul_small(cache[(size_t)i - 4], 1248u);
        cache[(size_t)i] = prod - cache[(size_t)i - 8];
    }
    computed = (computed < n ? n : computed);

    return cache[(size_t)n];
}
