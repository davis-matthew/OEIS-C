/*
 * A209378  1/4 the number of (n+1) X 5 0..2 arrays with every 2 X 2 subblock
 *          having distinct edge sums.
 * https://oeis.org/A209378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A209378 (OEIS) – 1/4 the number of (n+1)×5 0..2 arrays ...
 * Recurrence (order 8):
 *   a(n) = 3*a(n-1) + 12*a(n-2) - 42*a(n-3) - 24*a(n-4)
 *          + 156*a(n-5) - 48*a(n-6) - 168*a(n-7) + 112*a(n-8)
 *   with
 *     a(1)=32, a(2)=80, a(3)=156, a(4)=512, a(5)=1076, a(6)=4004,
 *     a(7)=8612, a(8)=33716.                                            */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamically built table */
static idx_t   cache_size = 0;       /* number of stored values   */
static idx_t   cache_cap  = 0;       /* current allocated capacity */

static void
grow_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1 */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap  = 8;
        cache_size = 8;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        /* initial values a(1)..a(8) */
        cache[0] = (seq_t)32;
        cache[1] = (seq_t)80;
        cache[2] = (seq_t)156;
        cache[3] = (seq_t)512;
        cache[4] = (seq_t)1076;
        cache[5] = (seq_t)4004;
        cache[6] = (seq_t)8612;
        cache[7] = (seq_t)33716;
    }

    /* extend cache up to n */
    if (n > cache_size) {
        grow_cache(n);
        for (idx_t idx = cache_size; idx < n; ++idx) {
            /* compute a(idx+1) using the recurrence:
               indices are zero‑based: a(idx+1)=cache[idx]
            */
            seq_t pos = (seq_t)3   * cache[idx-1]   /* a(n-1)   */
                      + (seq_t)12  * cache[idx-2]   /* a(n-2)   */
                      + (seq_t)156 * cache[idx-5]   /* a(n-5)   */
                      + (seq_t)112 * cache[idx-8];  /* a(n-8)   */

            seq_t neg = (seq_t)42  * cache[idx-3]   /* a(n-3)   */
                      + (seq_t)24  * cache[idx-4]   /* a(n-4)   */
                      + (seq_t)48  * cache[idx-6]   /* a(n-6)   */
                      + (seq_t)168 * cache[idx-7];  /* a(n-7)   */

            cache[idx] = pos - neg;  /* pos > neg for n ≥ 8 */
        }
        cache_size = n;
    }

    return cache[n-1];
}
