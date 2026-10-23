/*
 * A151912  Expansion of (1-8x-8x^3)/(1-2x+4x^2)^2.
 * https://oeis.org/A151912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=-4, a(2)=-28, a(3)=-56, a(4)=32, for n≥5:
 *            a(n)=4*a(n-1)-12*a(n-2)+16*a(n-3)-16*a(n-4)
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 25, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is derived from the generating function
   (1-8x-8x³)/(1-2x+4x²)²  */

#include <stdlib.h>

static const seq_t init[5] = { (seq_t)1, (seq_t)-4, (seq_t)-28, (seq_t)-56, (seq_t)32 };
static seq_t *cache = NULL;          /* memoized terms, cache[0..len-1] */
static idx_t cache_len = 0;          /* number of terms stored */

/* Return the n-th term of A151912. */
seq_t a(idx_t n)
{
    /* return directly if within the initial block */
    if (n < 5) return init[n];

    /* initialise cache the first time we need it */
    if (cache_len == 0) {
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < 5; ++i) cache[i] = init[i];
        if (!cache) return 0; /* allocation failed – unlikely for limits < 10000 */
    }

    /* grow cache if needed and compute new terms */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        /* compute terms from cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            /* recurrence: a(i)=4*a1-12*a2+16*a3-16*a4   */
            cache[i] = 4 * a1 - 12 * a2 + 16 * a3 - 16 * a4;
        }
        cache_len = new_len;
    }

    return cache[n];
}
