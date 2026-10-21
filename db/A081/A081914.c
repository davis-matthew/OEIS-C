/*
 * A081914  a(n) = 3^n*(n^3 - 3*n^2 + 2*n + 162)/162.
 * https://oeis.org/A081914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n*(n^3 - 3*n^2 + 2*n + 162)/162
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t  cache_len = 0;     /* current size of cache */

/* Compute a(n) for any n ≥ 0, caching intermediate results */
seq_t a(idx_t n)
{
    /* at least 4 elements so we can store the first four values */
    idx_t needed = (n >= 3) ? (n + 1) : 4;
    if (cache_len < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) { /* allocation failure – terminate the program */
            abort();
        }
        cache = tmp;
        for (idx_t i = cache_len; i < needed; ++i) cache[i] = 0;
        cache_len = needed;
    }

    /* Compute terms up to n if they are not yet known */
    for (idx_t i = 0; i <= n; ++i) {
        if (cache[i] != 0) continue;   /* already computed */

        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 3;
        } else if (i == 2) {
            cache[2] = 9;
        } else if (i == 3) {
            cache[3] = 28;
        } else { /* recurrence for i ≥ 4 */
            seq_t t1 = mul_small(cache[i - 1], 12);
            seq_t t3 = mul_small(cache[i - 3], 108);
            seq_t sum = t1 + t3;                /* (12*a[n-1] + 108*a[n-3]) */

            seq_t t2 = mul_small(cache[i - 2], 54);
            seq_t t4 = mul_small(cache[i - 4], 81);
            sum -= t2;                          /* - 54*a[n-2] */
            sum -= t4;                          /* - 81*a[n-4] */

            cache[i] = sum;
        }
    }

    return cache[n];
}
