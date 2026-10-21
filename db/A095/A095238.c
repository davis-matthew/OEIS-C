/*
 * A095238  a(1) = 1, a(n) = n*(sum of all previous terms mod n).
 * https://oeis.org/A095238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (sum_{k=1}^{n-1} a(k) mod n), a(1) = 1
 * Range:     u8 n <= 12, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoises all previously computed terms.  It is
   thread‑safe only on a single thread (as required by the judge). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                      /* out‑of‑range guard */
        return 0;

    /* Static cache */
    static seq_t *cache = NULL;
    static idx_t cached = 0;         /* highest index computed so far   */
    static seq_t sum_cum = 0;        /* sum_{k=1}^{cached} a(k)         */

    /* Ensure the first term is correct (a(1)=1) */
    if (n == 1) {
        if (cached < 1) {
            /* Allocate space for a[1] */
            cache = (seq_t *)malloc(sizeof(seq_t) * 1);
            cache[0] = (seq_t)1;
            sum_cum = cache[0];
            cached = 1;
        }
        return cache[0];
    }

    /* For n > 1 we need to compute up to n */
    if (n > cached) {
        /* Grow the cache to hold values up to n */
        idx_t new_size = n;
        if (cached == 0) {
            cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
        } else {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        }

        /* Compute missing values sequentially from cached+1 to n */
        for (idx_t i = cached + 1; i <= n; ++i) {
            seq_t mod_val = sum_cum % (seq_t)i;   /* sum modulo i */
            seq_t a_val = (seq_t)i * mod_val;     /* a(i)          */
            cache[i - 1] = a_val;                 /* store result  */
            sum_cum += a_val;                     /* update cumulative sum */
        }
        cached = n;
    }

    return cache[n - 1];
}
