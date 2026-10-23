/*
 * A189059  Number of n X 4 binary arrays without the pattern 0 1 0
 *          antidiagonally or horizontally.
 * https://oeis.org/A189059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) -20*a(n-2) -40*a(n-3) +64*a(n-4) +100*a(n-5)
 *            -176*a(n-6) +64*a(n-7) for n >= 8. Initial values: a(1)=12,
 *            a(2)=144, a(3)=1404, a(4)=13452, a(5)=128628, a(6)=1228512,
 *            a(7)=11733712
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed = 0;   /* largest n that has been computed */

    /* Initial values for n = 1..7 */
    static const seq_t init_vals[7] = {
        (seq_t)12,      /* a(1) */
        (seq_t)144,     /* a(2) */
        (seq_t)1404,    /* a(3) */
        (seq_t)13452,   /* a(4) */
        (seq_t)128628,  /* a(5) */
        (seq_t)1228512, /* a(6) */
        (seq_t)11733712 /* a(7) */
    };

    /* Ensure at least 7 slots are allocated */
    idx_t needed = (n > 7) ? n : (idx_t)7;
    if (cache_size < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_size = needed;
    }

    /* Compute values up to n if not already done */
    if (computed < n) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i <= 7) {  /* use the supplied initial values */
                cache[i - 1] = init_vals[i - 1];
            } else {       /* recurrence for i >= 8 */
                seq_t v = ((seq_t)12 * cache[i - 2])   /* 12*a(n-1) */
                        - ((seq_t)20 * cache[i - 3])   /* -20*a(n-2) */
                        - ((seq_t)40 * cache[i - 4])   /* -40*a(n-3) */
                        + ((seq_t)64 * cache[i - 5])   /* +64*a(n-4) */
                        + ((seq_t)100 * cache[i - 6])  /* +100*a(n-5) */
                        - ((seq_t)176 * cache[i - 7])  /* -176*a(n-6) */
                        + ((seq_t)64 * cache[i - 8]);  /* +64*a(n-7) */
                cache[i - 1] = v;
            }
        }
        computed = n;
    }

    return cache[n - 1];
}
