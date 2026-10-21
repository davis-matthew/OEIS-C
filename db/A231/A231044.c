/*
 * A231044  Number of n X 3 0..3 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4 and at least one element with value
 *          (x(i,j)-1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A231044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence (order 6) a(1)=0, a(2)=14,
 *            a(3)=50, a(4)=410, a(5)=2430, a(6)=16198
 *            a(n)=3*a(n-1)+19*a(n-2)+19*a(n-3)+12*a(n-4)-8*a(n-5)+8*a(n-6)
 *            for n>6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache array (1‑based). cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* number of elements allocated (>= cached_max+1) */
static idx_t cached_max = 0;   /* largest n for which cache[n] is valid */

/* Ensure cache is allocated for indices up to 'max_index' (inclusive). */
static void grow_cache(idx_t max_index)
{
    size_t needed = (size_t)max_index + 1;   /* indices 0..max_index */
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the first six known values. */
static void init_cache(void)
{
    grow_cache(6);                     /* need indices 0..6 */
    cache[0] = (seq_t)0;               /* unused */
    cache[1] = (seq_t)0;               /* a(1) */
    cache[2] = (seq_t)14;              /* a(2) */
    cache[3] = (seq_t)50;              /* a(3) */
    cache[4] = (seq_t)410;             /* a(4) */
    cache[5] = (seq_t)2430;            /* a(5) */
    cache[6] = (seq_t)16198;           /* a(6) */
    cached_max = 6;
}

/* Compute a(n) with memoisation and the recurrence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1, return 0 */

    if (cache == NULL)          /* first call */
        init_cache();

    if (cached_max < n) {       /* extend cache up to n */
        if ((idx_t)cache_cap - 1 < n)
            grow_cache(n);      /* ensure capacity for indices 0..n */

        for (idx_t i = cached_max + 1; i <= n; ++i) {
            seq_t value = 0;

            /* 3*a(i-1) */
            value += cache[i - 1] * (seq_t)3;
            /* 19*a(i-2) */
            value += cache[i - 2] * (seq_t)19;
            /* 19*a(i-3) */
            value += cache[i - 3] * (seq_t)19;
            /* 12*a(i-4) */
            value += cache[i - 4] * (seq_t)12;
            /* +8*a(i-6) */
            value += cache[i - 6] * (seq_t)8;
            /* -8*a(i-5) */
            value -= cache[i - 5] * (seq_t)8;

            cache[i] = value;
        }
        cached_max = n;
    }

    return cache[n];
}
