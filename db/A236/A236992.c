/*
 * A236992  Number of (n+1) X (3+1) 0..2 arrays with the maximum plus the
 *          lower median minus the upper median minus the minimum of every 2 X
 *          2 subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A236992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + 8704*a(n-4) - 87040*a(n-5) (for n ≥ 6)
 *            Initial values: a(1)=1600, a(2)=15552, a(3)=149632,
 *            a(4)=1513728, a(5)=14963200
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* store computed terms, index 0 → a(1) */
static idx_t  cache_cap = 0;       /* allocated capacity */
static idx_t  cache_size = 0;      /* number of terms stored */

/* grow the cache to hold at least `needed` terms */
static void ensure_cache(idx_t needed)
{
    if (cache_cap < needed)
    {
        idx_t newcap = cache_cap ? cache_cap * 2 : 16;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cache_cap = newcap;
    }
}

/* the requested sequence member */
seq_t a(idx_t n)
{
    /* array of the first five terms */
    static const seq_t init[5] = {
        (seq_t)1600,
        (seq_t)15552,
        (seq_t)149632,
        (seq_t)1513728,
        (seq_t)14963200
    };

    if (n <= 5) return init[n-1];

    if (cache_size < 5)
    {
        ensure_cache(5);
        for (idx_t i = cache_size; i < 5; ++i) cache[i] = init[i];
        cache_size = 5;
    }

    if (cache_size < n)
    {
        /* pre‑allocate storage for all needed terms */
        ensure_cache(n);

        /* compute terms from cache_size to n-1 (zero‑based index) */
        for (idx_t i = cache_size; i < n; ++i)
        {
            seq_t t1 = cache[i-1];     /* a(n-1) */
            seq_t t2 = cache[i-4];     /* a(n-4) */
            seq_t t3 = cache[i-5];     /* a(n-5) */

            seq_t val = 10 * t1;
            val += 8704 * t2;
            val -= 87040 * t3;
            cache[i] = val;
        }
        cache_size = n;
    }

    return cache[n-1];
}
