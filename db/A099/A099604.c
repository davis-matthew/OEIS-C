/*
 * A099604  Antidiagonal sums of triangle A099602, in which row n equals the
 *          inverse binomial transform of column n of the triangle of
 *          trinomial coefficients (A027907).
 * https://oeis.org/A099604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + 3*a(n-3) - 2*a(n-5) - a(n-6); a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=7, a(5)=12; (for n≥6)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already-computed terms */
    static seq_t *cache = NULL;        /* array with computed values */
    static idx_t cache_len = 0;        /* number of elements stored   */

    /* Initial values for n = 0..5 */
    static const seq_t init[6] = { (seq_t)1, (seq_t)1, (seq_t)2,
                                   (seq_t)4, (seq_t)7, (seq_t)12 };

    /* Make sure the cache contains enough space for index n */
    if (n >= cache_len) {
        /* Determine new capacity: at least n+1, growth factor 2 */
        idx_t new_len = n + 1;
        idx_t cap = (cache_len == 0) ? 16 : cache_len;
        while (cap < new_len) cap <<= 1;

        if (cache == NULL)
            cache = (seq_t*)malloc(cap * sizeof(seq_t));
        else
            cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));

        /* Initialise new entries if needed */
        if (cache_len == 0) {
            /* Ensure first 6 entries are set */
            for (idx_t i = 0; i < 6 && i < new_len; ++i)
                cache[i] = init[i];
        } else if (cache_len < 6) {
            /* Fill missing initial terms */
            for (idx_t i = cache_len; i < 6 && i < new_len; ++i)
                cache[i] = init[i];
        }

        /* Compute terms beyond what was previously stored */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i < 6)
                continue;          /* already initialized */

            /* a(i) = 2*a(i-2) + 3*a(i-3) - 2*a(i-5) - a(i-6) */
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a5 = cache[i-5];
            seq_t a6 = cache[i-6];

            seq_t sum = (seq_t)2 * a2 + (seq_t)3 * a3;
            seq_t sub = (seq_t)2 * a5 + a6;

            cache[i] = sum - sub;
        }
        cache_len = new_len;
    }

    return cache[n];
}
