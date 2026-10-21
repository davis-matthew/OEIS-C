/*
 * A235275  Number of (n+1) X (5+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 22, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235275: Number of (n+1)×(6) arrays with entries 0..4
 * such that every 2×2 subblock has its diagonal sum differing from its
 * antidiagonal sum by 5.
 *
 * Recurrence (empirical):
 *   a(n) =  a(n-1) + 15*a(n-2) - 15*a(n-3) - 80*a(n-4) + 80*a(n-5)
 *          + 180*a(n-6) - 180*a(n-7) - 144*a(n-8) + 144*a(n-9)
 * Initial values (n = 1 … 9):
 *   1120, 1660, 2344, 3952, 6232, 11704, 20440, 41704, 79480
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed values.  The cache indices are
     * zero‑based, so a(1) is stored at cache[0]. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of items currently stored */
    static size_t cap = 0;        /* allocated capacity of cache */

    /* Guard against the unused offset 0. */
    if (n == 0)
        return 0;

    /* Initialise the cache with the first nine values if necessary. */
    if (cache_len < 9) {
        size_t needed = 9;
        if (needed > cap) {
            seq_t *newptr = (seq_t*)realloc(cache, needed * sizeof(seq_t));
            if (!newptr) return 0;  /* out of memory – unlikely in tests */
            cache = newptr;
            cap = needed;
        }
        const seq_t init[9] = {
            1120, 1660, 2344, 3952, 6232,
            11704, 20440, 41704, 79480
        };
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init[i];
        cache_len = 9;
    }

    /* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute successive terms until reaching n. */
    for (idx_t cur = cache_len + 1; cur <= n; ++cur) {
        /* Separate positive and negative contributions to avoid
         * underflow with unsigned arithmetic.  All terms are
         * non‑negative, and the final result is positive. */
        seq_t pos = 0, neg = 0;

        /* a(cur-1) * +1 */
        pos += cache[cur - 2];
        /* a(cur-2) * +15 */
        pos += 15 * cache[cur - 3];
        /* a(cur-3) * -15 */
        neg += 15 * cache[cur - 4];
        /* a(cur-4) * -80 */
        neg += 80 * cache[cur - 5];
        /* a(cur-5) * +80 */
        pos += 80 * cache[cur - 6];
        /* a(cur-6) * +180 */
        pos += 180 * cache[cur - 7];
        /* a(cur-7) * -180 */
        neg += 180 * cache[cur - 8];
        /* a(cur-8) * -144 */
        neg += 144 * cache[cur - 9];
        /* a(cur-9) * +144 */
        pos += 144 * cache[cur - 10];

        seq_t val = pos - neg;

        /* Ensure cache has enough capacity. */
        if (cur > cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            if (new_cap < (size_t)cur) new_cap = cur;
            seq_t *newptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!newptr) return 0;  /* out of memory */
            cache = newptr;
            cap = new_cap;
        }

        cache[cur - 1] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
