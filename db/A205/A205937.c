/*
 * A205937  Number of (n+1) X 2 0..3 arrays with no 2 X 2 subblock having the
 *          number of clockwise edge increases equal to the number of
 *          counterclockwise edge increases in its adjacent leftward or upward
 *          neighbors.
 * https://oeis.org/A205937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 16*a(n-2) + 132*a(n-3) + 626*a(n-4) + 1504*a(n-5) +
                    2204*a(n-6) + 2140*a(n-7) + 1011*a(n-8) + 96*a(n-9) + 80*a(n-10)
   Initial values: a(1)=256, a(2)=1776, a(3)=12712, a(4)=91440,
                   a(5)=656272, a(6)=4711264, a(7)=33827496,
                   a(8)=242859696, a(9)=1743558448, a(10)=12517828752 */

#include <stdlib.h>

/* Cached sequence values. cache[i] holds a(i+1). */
static seq_t *cache      = NULL;
static size_t cache_size = 0;   /* number of values actually stored in cache */
static size_t cache_cap  = 0;   /* allocation capacity of cache */

/* Grow the cache to accommodate 'needed' values.  The function is
   safe for both C and C++ builds. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();     /* out of memory */
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Initial ten terms (seed for the recurrence). */
static const seq_t init_vals[10] = {
    (seq_t)256,          /* a(1) */
    (seq_t)1776,         /* a(2) */
    (seq_t)12712,        /* a(3) */
    (seq_t)91440,        /* a(4) */
    (seq_t)656272,       /* a(5) */
    (seq_t)4711264,      /* a(6) */
    (seq_t)33827496,     /* a(7) */
    (seq_t)242859696,    /* a(8) */
    (seq_t)1743558448,   /* a(9) */
    (seq_t)12517828752   /* a(10) */
};

/* Returns the n-th term of the sequence (with offset 1).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; a(0) undefined */

    /* Initialise cache if it is empty. */
    if (cache_size == 0) {
        grow_cache(10);
        for (size_t i = 0; i < 10; ++i) cache[i] = init_vals[i];
        cache_size = 10;
    }

    /* Return cached value if already computed. */
    if (n <= (idx_t)cache_size) return cache[n-1];

    /* Compute successive terms up to n. */
    size_t start  = cache_size;          /* zero‑based index of next value to compute */
    size_t target = (size_t)n;           /* zero‑based index of last value to compute */
    grow_cache(target);

    for (size_t i = start; i < target; ++i) {
        /* i is the zero‑based index corresponding to term a(i+1) */
        seq_t val =
            (seq_t)16   * cache[i-2] +
            (seq_t)132  * cache[i-3] +
            (seq_t)626  * cache[i-4] +
            (seq_t)1504 * cache[i-5] +
            (seq_t)2204 * cache[i-6] +
            (seq_t)2140 * cache[i-7] +
            (seq_t)1011 * cache[i-8] +
            (seq_t)96   * cache[i-9] +
            (seq_t)80   * cache[i-10];
        cache[i] = val;
    }
    cache_size = target;
    return cache[n-1];
}
