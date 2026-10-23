/*
 * A125371  Number of base 8 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=8, a(2)=58, a(3)=398, a(4)=2902, a(5)=21238;
 * for n≥6: a(n)=8*a(n-1)+a(n-4)+4*a(n-5)-(3*a(n-2)+14*a(n-3))
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[6] = {1, 8, 58, 398, 2902, 21238};

    /* base cases */
    if (n < 6)
        return init[n];

    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static idx_t last_computed = 0;

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
        last_computed = 5;
    }

    /* enlarge cache if necessary */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)   /* allocation failed – should not happen in tests */
            return init[0];
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms */
    if (n > last_computed) {
        for (idx_t k = last_computed + 1; k <= n; ++k) {
            seq_t tpos = 0;
            tpos += 8 * cache[k - 1];   /* 8*a(k-1) */
            tpos += cache[k - 4];       /* + a(k-4)       */
            tpos += 4 * cache[k - 5];   /* + 4*a(k-5)     */

            seq_t tneg = 0;
            tneg += 3 * cache[k - 2];   /* 3*a(k-2)       */
            tneg += 14 * cache[k - 3];  /* 14*a(k-3)      */

            cache[k] = tpos - tneg;
        }
        last_computed = n;
    }

    return cache[n];
}
