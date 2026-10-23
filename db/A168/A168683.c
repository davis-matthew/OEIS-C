/*
 * A168683  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A168683.
 *
 *  Recurrence:
 *    a(0) = 1 ,  a(1) = 6 ,
 *    a(n) = 5*a(n-1)                for 2 ≤ n ≤ 16 ,
 *    a(17) = 5*a(16)  - 14*a(0)  - 1 ,
 *    a(18) = 5*a(17)  - 14*a(1) + 10*a(0) - 1 ,
 *    a(n) = 5*a(n-1) + 10*a(n-18) - 14*a(n-17)   for n ≥ 19 .
 *
 *  This implements the recurrence directly, caching previously computed
 *  values.  It works for any index n ≥ 0 and any seq_t/idx_t type supplied
 *  by the build environment.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static int initialized = 0;
    static idx_t last = 0;
    static seq_t *cache = NULL;

    /* Ensure cache large enough. */
    if (!initialized) {
        /* First call: allocate enough space and compute all terms up to n. */
        cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (!cache) return (seq_t)0;  /* allocation failure; return 0 */

        for (idx_t i = 0; i <= n; ++i) {
            if (i == 0)
                cache[0] = (seq_t)1;
            else if (i == 1)
                cache[1] = (seq_t)6;
            else if (i >= 2 && i <= 16)
                cache[i] = (seq_t)5 * cache[i - 1];
            else if (i == 17)
                cache[i] = (seq_t)5 * cache[16] -
                           (seq_t)14 * cache[0] -
                           (seq_t)1;
            else if (i == 18)
                cache[i] = (seq_t)5 * cache[17] -
                           (seq_t)14 * cache[1] +
                           (seq_t)10 * cache[0] -
                           (seq_t)1;
            else /* i ≥ 19 */
                cache[i] = (seq_t)5 * cache[i - 1] +
                           (seq_t)10 * cache[i - 18] -
                           (seq_t)14 * cache[i - 17];
        }

        last = n;
        initialized = 1;
        return cache[n];
    }

    /* Subsequent calls. */
    if (n <= last)
        return cache[n];

    /* Resize cache if needed. */
    if (n + 1 > last + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
    }

    /* Compute new terms from last+1 up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = (seq_t)6;
        else if (i >= 2 && i <= 16)
            cache[i] = (seq_t)5 * cache[i - 1];
        else if (i == 17)
            cache[i] = (seq_t)5 * cache[16] -
                       (seq_t)14 * cache[0] -
                       (seq_t)1;
        else if (i == 18)
            cache[i] = (seq_t)5 * cache[17] -
                       (seq_t)14 * cache[1] +
                       (seq_t)10 * cache[0] -
                       (seq_t)1;
        else /* i ≥ 19 */
            cache[i] = (seq_t)5 * cache[i - 1] +
                       (seq_t)10 * cache[i - 18] -
                       (seq_t)14 * cache[i - 17];
    }

    last = n;
    return cache[n];
}
