/*
 * A192772  Constant term in the reduction of the n-th Fibonacci polynomial by
 *          x^3->x^2+2x+1.
 * https://oeis.org/A192772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+5*a(n-2)-a(n-3)-5*a(n-4)+a(n-5)+a(n-6);
   Initial values: a(1)=1, a(2)=0, a(3)=1, a(4)=1, a(5)=2, a(6)=7 */

#include <stdlib.h>

/* Cached sequence values (zero‑based indices: cache[0] = a(1)) */
static seq_t *cache    = NULL;  /* dynamic array of values */
static idx_t cache_size = 0;    /* allocated size of cache array */
static idx_t cache_used = 0;    /* number of values currently valid in cache */

/* Allocate and initialise the first six terms */
static void init_cache(void)
{
    if (cache_used != 0) return;          /* already initialised */

    /* Allocate small initial block and grow as needed */
    cache_size = 16;
    cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
    if (!cache) exit(1);                  /* out of memory */

    cache[0] = (seq_t)1;   /* a(1) */
    cache[1] = (seq_t)0;   /* a(2) */
    cache[2] = (seq_t)1;   /* a(3) */
    cache[3] = (seq_t)1;   /* a(4) */
    cache[4] = (seq_t)2;   /* a(5) */
    cache[5] = (seq_t)7;   /* a(6) */
    cache_used = 6;
}

/* Grow the cache array so that it can hold at least n elements */
static void ensure_capacity(idx_t n)
{
    if (n < cache_size) return;
    idx_t new_size = cache_size;
    while (new_size <= n) new_size <<= 1;                     /* double until big enough */
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(1);                                  /* out of memory */
    cache = new_cache;
    cache_size = new_size;
}

/* Compute a(n) for any n >= 1, using memoisation */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n < 1 */

    init_cache();
    if (n > cache_used) {
        ensure_capacity(n);
        /* Compute missing terms up to n */
        for (idx_t i = cache_used; i < n; ++i) {
            seq_t t1 = cache[i-1];
            seq_t t2 = cache[i-2];
            seq_t t3 = cache[i-3];
            seq_t t4 = cache[i-4];
            seq_t t5 = cache[i-5];
            seq_t t6 = cache[i-6];
            cache[i] =  t1
                        + (seq_t)5 * t2
                        - t3
                        - (seq_t)5 * t4
                        + t5
                        + t6;
        }
        cache_used = n;
    }
    return cache[n-1];
}
