/*
 * A118891  Number of binary sequences of length n with no subsequence 01110.
 * https://oeis.org/A118891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-4) + 4*a(n-5) - a(n-6) -
 *            a(n-7) + a(n-9) Initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=8,
 *            a(4)=16, a(5)=31, a(6)=60, a(7)=116, a(8)=223
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[9] = {1,2,4,8,16,31,60,116,223};

seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries stored in cache */

    /* Grow cache if necessary */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;         /* allocation failure (unlikely) */
        cache = tmp;
        /* Fill newly allocated part with initial values or recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 9) {
                cache[i] = init_vals[i];
            } else {
                /* Recurrence using only additions and subtractions to avoid
                   negative intermediate values (all indices < i are already
                   computed and thus non‑negative).              */
                seq_t pos = cache[i-1];
                pos += cache[i-1];      /* + a(n-1)  */
                pos += cache[i-1];      /* + a(n-1)  */
                /* + 4*a(n-5) */
                pos += cache[i-5];
                pos += cache[i-5];
                pos += cache[i-5];
                pos += cache[i-5];
                pos += cache[i-9];       /* + a(n-9)  */

                seq_t neg = cache[i-2];
                neg += cache[i-2];      /* -2*a(n-2) */
                neg += cache[i-4];
                neg += cache[i-4];      /* -2*a(n-4) */
                neg += cache[i-6];      /* - a(n-6)  */
                neg += cache[i-7];      /* - a(n-7)  */

                cache[i] = pos - neg;   /* pos > neg by construction */
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
