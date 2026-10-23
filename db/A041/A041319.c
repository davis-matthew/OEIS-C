/*
 * A041319  Denominators of continued fraction convergents to sqrt(173).
 * https://oeis.org/A041319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041319: a(n) = 2236*a(n-5) + a(n-10)  for n ≥ 10
   Initial values: a(0)=1, a(1)=6, a(2)=7, a(3)=13, a(4)=85,
   a(5)=2223, a(6)=13423, a(7)=15646, a(8)=29069, a(9)=190060
**/

#include <stdlib.h>   /* malloc, realloc, exit, EXIT_FAILURE */
#include <stdint.h>   /* for uintptr_t */

/* internal cache of computed terms */
static seq_t *cache = 0;      /* pointer to first cached value      */
static idx_t cache_size = 0;  /* number of cached values (next free) */

/* initialise the cache with the first ten terms */
static void init_cache(void)
{
    if (cache != 0) return;              /* already initialised */

    /* allocate memory for ten terms */
#ifdef __cplusplus
    cache = new seq_t[10];
#else
    cache = (seq_t *)malloc(10 * sizeof(seq_t));
    if (cache == 0) exit(EXIT_FAILURE);
#endif

    /* set the base values */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)6;
    cache[2] = (seq_t)7;
    cache[3] = (seq_t)13;
    cache[4] = (seq_t)85;
    cache[5] = (seq_t)2223;
    cache[6] = (seq_t)13423;
    cache[7] = (seq_t)15646;
    cache[8] = (seq_t)29069;
    cache[9] = (seq_t)190060;

    cache_size = 10;
}

/* compute a(n) using recurrence with memoisation */
seq_t a(idx_t n)
{
    init_cache();                           /* ensure base terms exist */

    /* return the cached value if already known */
    if (n < cache_size) return cache[n];

    /* grow the cache up to index n */
    idx_t new_size = n + 1;
    seq_t *new_cache = 0;

#ifdef __cplusplus
    /* C++: allocate new array and copy old values */
    new_cache = new seq_t[new_size];
    for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
    delete [] cache;
    cache = new_cache;
#else
    /* C: use realloc to extend the block */
    new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == 0) exit(EXIT_FAILURE);
    cache = new_cache;
#endif

    /* constant factor for the recurrence */
    const seq_t C1 = (seq_t)2236;

    /* fill in missing terms using the recurrence */
    for (idx_t i = cache_size; i < new_size; ++i) {
        cache[i] = C1 * cache[i - 5] + cache[i - 10];
    }

    cache_size = new_size;
    return cache[n];
}
