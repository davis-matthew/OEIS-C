/*
 * A211575  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two,
 *          three, four or six distinct values for every i,j,k<=n.
 * https://oeis.org/A211575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 3*a(n-2) - 46*a(n-3) + 88*a(n-4) + 49*a(n-5) -
 *            231*a(n-6) + 82*a(n-7) + 168*a(n-8) - 100*a(n-9) - 36*a(n-10) +
 *            24*a(n-11) Initial values (n = 1 .. 11): 24, 80, 202, 476, 1082,
 *            2470, 5562, 12796, 29044, 67738, 155328 (Recurrent formula holds
 *            for n ≥ 12)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Return 0 for out–of–range indices (offset is 1). */
    if (n == 0) return (seq_t)0;

    /* Static cache for memoisation across calls. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initial 11 terms for n = 1 .. 11. */
    static const seq_t init_vals[11] = {
        24, 80, 202, 476, 1082,
        2470, 5562, 12796, 29044, 67738, 155328
    };

    /* Grow the cache to contain at least n terms. */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort. */
            abort();
        }
        cache = tmp;

        /* Compute values from old_len to new_len-1. */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i < 11) {
                /* Use the hard‑coded initial values. */
                cache[i] = init_vals[i];
            } else {
                /* Compute by the recurrence without signed intermediates. */
                seq_t pos = cache[i-1]*6    /* 6*a(n-1)   */
                          + cache[i-4]*88   /* 88*a(n-4)  */
                          + cache[i-5]*49   /* 49*a(n-5)  */
                          + cache[i-7]*82   /* 82*a(n-7)  */
                          + cache[i-8]*168  /* 168*a(n-8) */
                          + cache[i-11]*24; /* 24*a(n-11) */

                seq_t neg = cache[i-2]*3   /* 3*a(n-2)   */
                          + cache[i-3]*46  /* 46*a(n-3)  */
                          + cache[i-6]*231 /* 231*a(n-6) */
                          + cache[i-9]*100 /* 100*a(n-9) */
                          + cache[i-10]*36;/* 36*a(n-10)*/

                cache[i] = pos - neg;
            }
        }
        cache_len = new_len;
    }

    return cache[n-1];
}
