/*
 * A237630  Number of (n+1) X (1+1) 0..2 arrays with the upper median of every
 *          2 X 2 subblock differing from its horizontal and vertical
 *          neighbors by exactly one.
 * https://oeis.org/A237630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*a(n-4) + 48*a(n-6) for n>9, with a(1)=81, a(2)=260,
 *            a(3)=702, a(4)=1786, a(5)=4621, a(6)=11936, a(7)=30656,
 *            a(8)=77576, a(9)=200452
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 19, u64 n <= 43, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation storage */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity (in elements) */
static size_t cache_len = 0;   /* number of values actually computed */

/* Initial values for n = 1..9 (index 0..8). */
static const seq_t init[9] = {
    81,     /* a(1) */
    260,    /* a(2) */
    702,    /* a(3) */
    1786,   /* a(4) */
    4621,   /* a(5) */
    11936,  /* a(6) */
    30656,  /* a(7) */
    77576,  /* a(8) */
    200452  /* a(9) */
};

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();  /* out of memory */

    cache = tmp;
    cache_cap = new_cap;
}

/* External function computing the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for 0; return 0 */

    size_t idx = (size_t)n;
    if (idx <= cache_len)
        return cache[idx - 1];   /* already computed */

    ensure_capacity(idx);

    /* Compute missing terms from cache_len up to idx-1. */
    for (size_t i = cache_len; i < idx; ++i) {
        if (i < 9) {
            cache[i] = init[i];       /* a(i+1) from the initial list */
        } else {
            cache[i] = 36 * cache[i - 4] + 48 * cache[i - 6];
        }
    }
    cache_len = idx;

    return cache[idx - 1];
}
