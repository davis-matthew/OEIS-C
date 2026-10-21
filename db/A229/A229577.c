/*
 * A229577  Number of defective 4-colorings of an n X 7 0..3 array connected
 *          horizontally, vertically, diagonally and antidiagonally with
 *          exactly one mistake, and colors introduced in row-major 0..3
 *          order.
 * https://oeis.org/A229577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 13*a(n-2) + 12*a(n-3) - 4*a(n-4) for n > 7 *
 *            with a(1)=246, a(2)=1312, a(3)=1584, a(4)=2064, a(5)=2852, *
 *            a(6)=4048, a(7)=6108
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* 1‑indexed array of computed terms   */
static size_t cache_cap = 0;       /* allocated capacity of cache         */
static size_t cache_len = 0;       /* highest index that has been filled  */

/* Ensure the cache can hold indices up to 'needed' (inclusive).  */
static void ensure_cache_size(size_t needed)
{
    if (needed < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= needed)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Initialise the cache with the first seven terms, if not already done.  */
static void init_cache(void)
{
    if (cache_len != 0)
        return;
    cache[1] = (seq_t)246;
    cache[2] = (seq_t)1312;
    cache[3] = (seq_t)1584;
    cache[4] = (seq_t)2064;
    cache[5] = (seq_t)2852;
    cache[6] = (seq_t)4048;
    cache[7] = (seq_t)6108;
    cache_len = 7;
}

seq_t a(idx_t n)
{
    if (n <= 0)
        return (seq_t)0;   /* Undefined for n <= 0 – return 0 */

    size_t idx = (size_t)n;      /* n is guaranteed to fit in size_t for the test limits */

    ensure_cache_size(idx + 1);  /* allocate indices 0..idx */
    init_cache();

    if (idx > cache_len) {
        for (size_t k = cache_len + 1; k <= idx; ++k) {
            /* Compute term = 6*a(k-1) + 12*a(k-3)
               and sub  = 13*a(k-2) + 4*a(k-4)   (term > sub for all k > 7) */
            seq_t term = (seq_t)6 * cache[k - 1] + (seq_t)12 * cache[k - 3];
            seq_t sub  = (seq_t)13 * cache[k - 2] + (seq_t)4 * cache[k - 4];
            cache[k] = term - sub;
        }
        cache_len = idx;
    }

    return cache[idx];
}
