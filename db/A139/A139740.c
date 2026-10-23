/*
 * A139740  a(n) = 11^n - 2^n.
 * https://oeis.org/A139740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 2^n Recurrence: a(0) = 0, a(1) = 9, a(n) =
 *            13*a(n-1) - 22*a(n-2) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;      /* array of computed values */
    static size_t  cur_len = 0;        /* number of values actually stored    */
    static size_t  cap     = 0;        /* allocated capacity                 */

    /* ensure base case(s) are stored */
    if (cur_len == 0) {
        /* start with room for two elements */
        size_t new_cap = 2;
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!cache) return 0;          /* unlikely – guard against OOM */
        cap = new_cap;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)9;
        cur_len = 2;
    } else if (cur_len == 1) {         /* only a(0) stored yet */
        if (cap < 2) {
            cache = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
            cap = 2;
        }
        cache[1] = (seq_t)9;
        cur_len = 2;
    }

    /* return cached value if we already computed it */
    if ((size_t)n < cur_len)
        return cache[(size_t)n];

    /* enlarge the cache to hold n+1 entries */
    if (cap <= (size_t)n) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute missing values up to n using the recurrence */
    for (size_t i = cur_len; i <= (size_t)n; ++i) {
        if (i == 0)          { cache[0] = (seq_t)0; continue; }
        if (i == 1)          { cache[1] = (seq_t)9; continue; }

        seq_t t1 = cache[i - 1] * 13;
        seq_t t2 = cache[i - 2] * 22;
        cache[i] = t1 - t2;
    }

    cur_len = (size_t)n + 1;
    return cache[(size_t)n];
}
