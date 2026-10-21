/*
 * A230063  Number of nX3 0..2 arrays x(i,j) with each element diagonally or
 *          antidiagonally next to at least one element with value 2-x(i,j).
 * https://oeis.org/A230063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A230063 – Number of n×3 arrays with diagonal antidiagonal adjacency condition
 *
 *  Recurrence (empirical):
 *      a(1) = 0
 *      a(2) = 9
 *      a(3) = 45
 *      a(n) = 7*a(n‑1) + 14*a(n‑2) – 8*a(n‑3)   for n ≥ 4
 *
 *  The function below stores computed values in a static cache so that
 *  calls in any order run in O(1) time per new value and O(n) memory.
 *  All arithmetic is performed with the build‑supplied integer type
 *  seq_t, which may be fixed‑size or arbitrary precision.
 */

#include <stdlib.h>

static seq_t *cache = NULL;         /* cache[0] is unused */
static size_t cache_cap = 0;        /* maximum index currently allocated */
static idx_t cache_max  = 0;        /* largest n whose value is in the cache */

/* Ensure that indices up to n are addressable in cache. */
static void ensure_cache(idx_t n)
{
    if (n > cache_cap) {
        /* grow aggressively to keep reallocs rare */
        size_t new_cap = n * 2 + 10;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (!tmp)
            abort();                /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Lazy initialization of the first three terms. */
static void init_cache(void)
{
    ensure_cache(3);
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)9;
    cache[3] = (seq_t)45;
    cache_max = 3;
}

/* Public entry point: sequence value a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)         /* offset‑1 fallback; not part of OEIS sequence */
        return (seq_t)0;

    if (!cache)
        init_cache();

    /* Extend the cache up to the requested index. */
    if (n > cache_max) {
        ensure_cache(n);
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t term = (seq_t)7 * cache[i - 1]
                       + (seq_t)14 * cache[i - 2]
                       - (seq_t)8  * cache[i - 3];
            cache[i] = term;
        }
        cache_max = n;
    }

    return cache[n];
}
