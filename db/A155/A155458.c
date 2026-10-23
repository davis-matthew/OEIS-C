/*
 * A155458  a(n)=6*a(n-1)+25*a(n-2), n>2 ; a(0)=1, a(1)=1, a(2)=6 .
 * https://oeis.org/A155458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1)+25*a(n-2), a(0)=1, a(1)=1, a(2)=6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL; /* cached terms */
    static size_t cap = 0;      /* allocated array size */
    static idx_t cache_size = 0;/* number of terms stored (last index + 1) */

    /* initialise on first call */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* out‑of‑memory: return 0 as sentinel */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache_size = 3;
    }

    /* return value if already computed */
    if (n < cache_size)
        return cache[n];

    /* grow the cache if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* out‑of‑memory: return 0 as sentinel */
        cache = tmp;
    }

    /* compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t part1 = (seq_t)6 * cache[i - 1];
        seq_t part2 = (seq_t)25 * cache[i - 2];
        cache[i] = part1 + part2;
    }

    cache_size = n + 1;
    return cache[n];
}
