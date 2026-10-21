/*
 * A235947  Number of (n+1) X (1+1) 0..2 arrays colored with the upper median
 *          value of each 2 X 2 subblock.
 * https://oeis.org/A235947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-3) + 36*a(n-4) + 48*a(n-6) for n>9 Base values:
 *            a(1)=81, a(2)=294, a(3)=905, a(4)=2584, a(5)=7353, a(6)=20800,
 *            a(7)=58896, a(8)=167240, a(9)=474948
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 17, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;          /* indices below offset are irrelevant */

    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* array indexed 0…k */
    static idx_t  cached_last = 0;     /* largest n computed so far */
    static size_t cache_cap    = 0;     /* allocated size of cache */

    /* one‑time initialisation: reserve slots 0…9 (10 elements) */
    if (!cache) {
        cache_cap = 10;
        cache = seq_alloc(cache_cap);
        if (!cache) return (seq_t)0;           /* allocation failed */

        cache[0] = (seq_t)0;                   /* unused, but keeps indices aligned */
        cache[1] = (seq_t)81;
        cache[2] = (seq_t)294;
        cache[3] = (seq_t)905;
        cache[4] = (seq_t)2584;
        cache[5] = (seq_t)7353;
        cache[6] = (seq_t)20800;
        cache[7] = (seq_t)58896;
        cache[8] = (seq_t)167240;
        cache[9] = (seq_t)474948;
        cached_last = (idx_t)9;
    }

    /* grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) return (seq_t)0;           /* reallocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    if (n > cached_last) {
        for (idx_t i = cached_last + 1; i <= n; ++i) {
            if (i <= (idx_t)9) continue;      /* already initialised */
            cache[i] = cache[i-3] * (seq_t)8
                     + cache[i-4] * (seq_t)36
                     + cache[i-6] * (seq_t)48;
        }
        cached_last = n;
    }

    return cache[n];
}
