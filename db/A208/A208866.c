/*
 * A208866  Number of n X 2 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 55*a(n-2) + 42*a(n-3) - 9*a(n-4) (n>5)
 *            Initial values: a(1)=2, a(2)=12, a(3)=143, a(4)=1979, a(5)=28246
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* 1‑based array of computed terms        */
static idx_t  cache_size = 0;       /* allocated size (max index that may be stored) */
static idx_t  last_computed = 0;    /* largest n for which cache[n] is valid   */
static bool   initialized = false;  /* whether the cache has been initialized */

static void init_cache(void)
{
    if (initialized) return;
    cache_size = 16;   /* start with a modest size */
    cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
    if (!cache) abort();               /* memory must be available */
    for (idx_t i = 0; i <= cache_size; ++i) cache[i] = 0;
    /* Set the known initial values (1‑based indexing) */
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)12;
    cache[3] = (seq_t)143;
    cache[4] = (seq_t)1979;
    cache[5] = (seq_t)28246;
    last_computed = 5;
    initialized = true;
}

/* Ensure that the cache can hold indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if (n <= cache_size) return;
    idx_t new_size = cache_size ? cache_size << 1 : n;
    while (new_size < n) new_size <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
    if (!new_cache) abort();           /* memory must be available */
    cache = new_cache;
    /* New entries are uninitialized; they will be filled before use */
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1, return 0 */

    init_cache();

    if (n <= last_computed) return cache[n];

    ensure_cache_size(n);

    /* Compute terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t t1 = (seq_t)18 * cache[i - 1];
        seq_t t2 = (seq_t)55 * cache[i - 2];
        seq_t t3 = (seq_t)42 * cache[i - 3];
        seq_t t4 = (seq_t)9  * cache[i - 4];

        seq_t sum = t1 + t3;    /* positive contributions first   */
        sum -= t2;              /* subtract negative            */
        sum -= t4;              /* subtract last negative       */

        cache[i] = sum;
    }
    last_computed = n;
    return cache[n];
}
