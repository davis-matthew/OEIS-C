/*
 * A237379  Number of (n+1)X(3+1) 0..2 arrays with the maximum plus the
 *          minimum minus the upper median minus the lower median of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (valid for n > 11):  
   a(n) = 10380*a(n-4) - 26935616*a(n-8)  
   Initial values:  
   a(1)=1472, a(2)=12408, a(3)=104248, a(4)=912912, a(5)=7660840,  
   a(6)=64312576, a(7)=541081120, a(8)=4737777088, a(9)=39760171616,  
   a(10)=333806884096, a(11)=2808515986304 */
#include <stddef.h>
#include <stdlib.h>

/* Cache to store computed values. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Base values a(1) .. a(11). */
static const seq_t base[11] = {
    (seq_t)1472ULL,
    (seq_t)12408ULL,
    (seq_t)104248ULL,
    (seq_t)912912ULL,
    (seq_t)7660840ULL,
    (seq_t)64312576ULL,
    (seq_t)541081120ULL,
    (seq_t)4737777088ULL,
    (seq_t)39760171616ULL,
    (seq_t)333806884096ULL,
    (seq_t)2808515986304ULL
};

seq_t a(idx_t n)
{
    /* Return 0 for 0 index (outside offset). */
    if (n == 0) return (seq_t)0;

    /* Lazy allocate a reasonable initial cache size. */
    if (!cache) {
        cache_cap = 20000;                     /* plenty for n up to ~10⁴ */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
    }

    /* Initialise cache with known base values. */
    if (cache_len == 0) {
        for (size_t i = 0; i < 11; ++i)
            cache[i] = base[i];
        cache_len = 11;
    }

    /* Expand cache if we need more terms. */
    while (cache_len < (size_t)n) {
        /* Grow capacity if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= cache_len) new_cap = cache_len + 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;       /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute a(n) using the recurrence.  Index i = cache_len corresponds
           to a(i+1).  For i >= 11, a(i+1) = 10380*a(i-3) - 26935616*a(i-7). */
        seq_t val4 = cache[cache_len - 4];      /* a(n-4) */
        seq_t val8 = cache[cache_len - 8];      /* a(n-8) */
        cache[cache_len] = val4 * (seq_t)10380UL -
                           val8 * (seq_t)26935616UL;
        ++cache_len;
    }

    return cache[n - 1];
}
