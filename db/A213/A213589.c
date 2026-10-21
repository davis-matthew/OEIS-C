/*
 * A213589  Antidiagonal sums of the convolution array A213587.
 * https://oeis.org/A213589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-3) + 3*a(n-5) + a(n-6); a(1)=1, a(2)=6,
 *            a(3)=20, a(4)=55, a(5)=135, a(6)=308
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 33, u64 n <= 75, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>


static seq_t *cache      = NULL;   /* stores a[1] .. a[max]   */
static idx_t  cache_size = 0;      /* allocated size (>= 7)  */
static idx_t  last_computed = 0;   /* highest index computed */
static int    initialized  = 0;    /* has the cache been set up? */

/* Ensure cache is allocated and initialized */
static void init_cache(void) {
    if (initialized) return;
    cache_size = 8;                /* at least 7 entries needed */
    cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
    if (!cache) exit(1);           /* cannot recover – abort */
    cache[1] = 1;
    cache[2] = 6;
    cache[3] = 20;
    cache[4] = 55;
    cache[5] = 135;
    cache[6] = 308;
    last_computed = 6;
    initialized = 1;
}

/* Grow the cache to at least n+1 (since we ignore index 0) */
static void ensure_capacity(idx_t n) {
    if (n < cache_size) return;
    idx_t new_size = cache_size ? cache_size * 2 : 8;
    while (new_size <= n) new_size *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(1); /* allocation failed */
    if (new_size > cache_size)
        memset(new_cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
    cache = new_cache;
    cache_size = new_size;
}

/* Main sequence routine */
seq_t a(idx_t n) {
    if (n == 0) return 0;        /* offset starts at 1 */
    init_cache();
    if (n <= last_computed)     /* already computed */
        return cache[n];

    ensure_capacity(n);
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = 3 * cache[i - 1] - 5 * cache[i - 3] + 3 * cache[i - 5] + cache[i - 6];
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
