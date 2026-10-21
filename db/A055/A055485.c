/*
 * A055485  Number of unlabeled 3-element intersecting families (with distinct
 *          sets) of an n-element set.
 * https://oeis.org/A055485
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 99, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A055485
 *
 *  Formula: a(n) satisfies the linear recurrence
 *
 *    a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 2*a(n-4)
 *           + 4*a(n-5) + 3*a(n-6) - 12*a(n-7) + 3*a(n-8)
 *           + 4*a(n-9) + 2*a(n-10) - 2*a(n-11) - 4*a(n-12)
 *           + 4*a(n-13) - a(n-14)      for n ≥ 14
 *
 *  Initial values (indices 0 … 14):
 *    a(0)=a(1)=a(2)=0,   a(3)=4,   a(4)=19,  a(5)=61,
 *    a(6)=157, a(7)=353, a(8)=717, a(9)=1355, a(10)=2412,
 *    a(11)=4094, a(12)=6676, a(13)=10524, a(14)=16108 .
 *
 *  The function below memoises values in a static array and
 *  uses the recurrence for n ≥ 15.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* initialise the cache with the first 15 terms */
static void init_cache(void)
{
    if (cache) return;
    cache_size = 15;                     /* indices 0 … 14 are required */
    cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
    static const seq_t init_vals[15] = {
        0ULL, 0ULL, 0ULL,                 /* a(0), a(1), a(2) */
        4ULL, 19ULL, 61ULL, 157ULL, 353ULL,
        717ULL, 1355ULL, 2412ULL, 4094ULL, 6676ULL,
        10524ULL, 16108ULL                 /* a(13), a(14) */
    };
    for (size_t i = 0; i < cache_size; ++i)
        cache[i] = init_vals[i];
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n < 3) return 0;                /* below the offset – undefined */
    init_cache();                       /* ensure the base terms exist */

    if (n < cache_size)                 /* already computed */
        return cache[(size_t)n];

    /* extend the cache up to n */
    size_t old_size = cache_size;
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return 0;           /* allocation failure – return 0 */
    cache = new_cache;
    cache_size = new_size;

    /* recurrence starts from index 15 (a(0)…a(14) are known) */
    idx_t start = old_size;
    if (start <= 14) start = 15;

    for (idx_t i = start; i <= n; ++i)
    {
        seq_t val = 0;
        val += 4 * cache[(size_t)(i - 1)];
        val -= 4 * cache[(size_t)(i - 2)];
        val -= 2 * cache[(size_t)(i - 3)];
        val += 2 * cache[(size_t)(i - 4)];
        val += 4 * cache[(size_t)(i - 5)];
        val += 3 * cache[(size_t)(i - 6)];
        val -= 12 * cache[(size_t)(i - 7)];
        val += 3 * cache[(size_t)(i - 8)];
        val += 4 * cache[(size_t)(i - 9)];
        val += 2 * cache[(size_t)(i - 10)];
        val -= 2 * cache[(size_t)(i - 11)];
        val -= 4 * cache[(size_t)(i - 12)];
        val += 4 * cache[(size_t)(i - 13)];
        val -= cache[(size_t)(i - 14)];
        cache[(size_t)i] = val;
    }

    return cache[(size_t)n];
}
