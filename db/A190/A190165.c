/*
 * A190165  Number of peakless Motzkin paths of length n having no (1,0)-steps
 *          at levels 0,2,4,... .
 * https://oeis.org/A190165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence (n+2)a(n) + (-2n-1)a(n-1) +
 *            (n+1)a(n-2) + (-2n+1)a(n-3) + (-2n+11)a(n-5) + (n-7)a(n-6) +
 *            (-2n+13)a(n-7) + (n-8)a(n-8) = 0 with initial values a(0)=1,
 *            a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=1, a(6)=2, a(7)=4, a(8)=7
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t base[9] = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1,
                               (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)4,
                               (seq_t)7 };

seq_t a(idx_t n)
{
    /* Return the initial values directly for small indices. */
    if (n < 9)
        return base[n];

    /* Persistent cache for computed terms. */
    static seq_t *cache    = NULL;
    static size_t cache_sz = 0;
    static idx_t  computed = 8;   /* greatest index already stored */

    /* Grow the cache if needed. */
    if (n >= cache_sz) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        if (cache_sz < new_sz)
            memset(cache + cache_sz, 0,
                   (new_sz - cache_sz) * sizeof(seq_t));

        cache_sz = new_sz;
        /* Store the base values (they are needed for the first few indices). */
        if (cache_sz >= 9) {
            for (idx_t i = 0; i < 9; ++i)
                cache[i] = base[i];
        }
    }

    /* Compute from the next uncomputed index up to n. */
    for (idx_t k = computed + 1; k <= n; ++k) {
        /* Separate positive and negative contributions to avoid
           underflow of an unsigned intermediate. */
        seq_t pos = (seq_t)(2 * k + 1) * cache[k - 1];
        pos += (seq_t)(2 * k - 1) * cache[k - 3];
        pos += (seq_t)(2 * k - 11) * cache[k - 5];
        pos += (seq_t)(2 * k - 13) * cache[k - 7];

        seq_t neg = (seq_t)(k + 1) * cache[k - 2];
        neg += (seq_t)(k - 7) * cache[k - 6];
        neg += (seq_t)(k - 8) * cache[k - 8];

        cache[k] = (pos - neg) / (seq_t)(k + 2);  /* division is exact */
    }
    computed = n;
    return cache[n];
}
