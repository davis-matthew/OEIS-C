/*
 * A203447  Number of (n+1)X4 0..1 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) such that rows
 *          of b(i,j) and columns of c(i,j) are lexicographically
 *          nondecreasing.
 * https://oeis.org/A203447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 76*a(n-2) + 262*a(n-3) - 583*a(n-4) +
 *            847*a(n-5) - 726*a(n-6) + 132*a(n-7) + 561*a(n-8) - 869*a(n-9) +
 *            704*a(n-10) - 362*a(n-11) + 119*a(n-12) - 23*a(n-13) + 2*a(n-14)
 *            Initial values: a(1)=32, a(2)=80, a(3)=321, a(4)=1177,
 *            a(5)=4200, a(6)=13777, a(7)=42112, a(8)=120522, a(9)=32...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 20, u64 n <= 52, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Hard‑coded initial terms (index 0 unused). */
static const unsigned long long _a0_17_ull[18] = {
    0ULL, /* unused 0 */
    32ULL, 80ULL, 321ULL, 1177ULL, 4200ULL, 13777ULL, 42112ULL,
    120522ULL, 325918ULL, 838830ULL, 2069033ULL, 4920266ULL,
    11341301ULL, 25458409ULL, 55884995ULL, 120402167ULL, 255405453ULL
};

/* Memoisation cache for computed values. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated size in elements */
static idx_t  cache_max  = 0;   /* maximum index that is filled in cache */
static int    cache_init = 0;   /* flag indicating cache initialisation */

/* Allocate or grow cache to at least size 'min_cap' elements. */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();       /* graceful out-of-memory handling */

    cache = new_cache;
    cache_cap = new_cap;
}

/* */  
seq_t a(idx_t n)
{
    /* Return pre‑computed values for n <= 17 immediately. */
    if (n <= 17) {
        return (seq_t)_a0_17_ull[(size_t)n];
    }

    /* Initialise cache on first call. */
    if (!cache_init) {
        /* Allocate space for indices 0..17 (index 0 unused). */
        cache_cap = 18;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();

        for (size_t i = 1; i <= 17; ++i) {
            cache[i] = (seq_t)_a0_17_ull[i];
        }
        cache_max = 17;
        cache_init = 1;
    }

    /* Grow cache if required. */
    if ((size_t)(n + 1) > cache_cap) {
        grow_cache((size_t)(n + 1));
    }

    /* Fill cache entries from cache_max+1 up to n. */
    for (size_t m = (size_t)(cache_max + 1); m <= (size_t)n; ++m) {
        /* Compute positive and negative parts of the recurrence separately
           to avoid unsigned underflow. The final result is guaranteed
           non‑negative. */
        seq_t pos = (seq_t)13 * cache[m - 1] +
                    (seq_t)262 * cache[m - 3] +
                    (seq_t)847 * cache[m - 5] +
                    (seq_t)132 * cache[m - 7] +
                    (seq_t)561 * cache[m - 8] +
                    (seq_t)704 * cache[m -10] +
                    (seq_t)119 * cache[m -12] +
                    (seq_t)2   * cache[m -14];

        seq_t neg = (seq_t)76  * cache[m - 2] +
                    (seq_t)583 * cache[m - 4] +
                    (seq_t)726 * cache[m - 6] +
                    (seq_t)869 * cache[m - 9] +
                    (seq_t)362 * cache[m -11] +
                    (seq_t)23  * cache[m -13];

        cache[m] = pos - neg;   /* final positive result */
    }

    cache_max = n;
    return cache[(size_t)n];
}
