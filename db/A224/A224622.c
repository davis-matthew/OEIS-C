/*
 * A224622  Number of (n+4) X 9 0..2 matrices with each 5 X 5 subblock
 *          idempotent.
 * https://oeis.org/A224622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 49, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A224622
 *
 *  Formula:
 *    a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) + a(n-5) - 3*a(n-6)
 *           + 3*a(n-7) - a(n-8)    for n > 12
 *
 *  Initial values:
 *    a(1)  = 39001
 *    a(2)  = 23453
 *    a(3)  = 28058
 *    a(4)  = 30723
 *    a(5)  = 32204
 *    a(6)  = 33115
 *    a(7)  = 41120
 *    a(8)  = 53862
 *    a(9)  = 69089
 *    a(10) = 85561
 *    a(11) = 102702
 *    a(12) = 127605
 */

#include <stdlib.h>
#include <stddef.h>

/*--- memoisation --------------------------------------------------------*/

static const seq_t init_seq[13] = {
    0,          /* unused index 0 */
    39001, 23453, 28058, 30723, 32204,
    33115, 41120, 53862, 69089, 85561,
    102702, 127605
};

static seq_t *cache   = NULL;   /* array 0..current_max */
static idx_t  cache_len = 0;     /* highest index stored (>=0) */

/* Grow cache to hold indices up to n.  If cache already large enough,
 * do nothing.  Otherwise allocate a larger array, copy any existing
 * values, and compute new terms using the recurrence. */
static void _ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    size_t new_len = (size_t)n + 1;      /* indices 0..n inclusive */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        return;                          /* allocation failed → cannot compute */
    cache = tmp;

    /* If we are just starting (or had less than 12 entries), fill init */
    if (cache_len < 13) {
        for (idx_t i = cache_len + 1; i <= 12 && i <= n; ++i)
            cache[i] = init_seq[i];
    }

    /* Compute new terms, starting after any already known ones */
    idx_t start = (cache_len > 12 ? cache_len : 12) + 1;
    for (idx_t i = start; i <= n; ++i) {
        seq_t t1 = 4 * cache[i-1];
        seq_t t2 = 6 * cache[i-2];
        seq_t t3 = 4 * cache[i-3];
        seq_t t4 = cache[i-4];
        seq_t t5 = cache[i-5];
        seq_t t6 = 3 * cache[i-6];
        seq_t t7 = 3 * cache[i-7];
        seq_t t8 = cache[i-8];
        cache[i] = t1 - t2 + t3 - t4 + t5 - t6 + t7 - t8;
    }

    cache_len = n;
}

/*--- public interface ---------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    if (n <= 12)
        return init_seq[n];

    _ensure_cache(n);
    return cache[n];
}
