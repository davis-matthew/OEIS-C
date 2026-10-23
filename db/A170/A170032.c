/*
 * A170032  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170032.
 *  a(n) satisfies the linear recurrence
 *     a(n) = 21*(a(n-1)+…+a(n-35)) – 231*a(n-36) + b(n)   (n ≥ 0)
 *  where                b(0) = 1,  b(1)…b(35) = 2,  b(36) = 1,
 *        and b(n) = 0 for n > 36.
 *  The recurrence follows from the generating function
 *      (t^36 + 2t^35 + … + 2t + 1) /
 *      (231t^36 – 21t^35 – … – 21t + 1).
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;   /* allocated capacity   */
    static idx_t  len   = 0;   /* number of filled terms 0 .. len-1 */

    /* enlarge cache if necessary */
    if (n >= len) {
        idx_t new_len = n + 1;
        if (new_len > cap) {
            idx_t new_cap = cap ? cap << 1 : new_len;
            if (new_cap < new_len) new_cap = new_len;
            if (cache == NULL)
                cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            else
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (cache == NULL) return (seq_t)0; /* allocation failure */
            cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = len; i <= n; ++i) {
            seq_t s = 0;
            /* sum of last up to 35 previous terms */
            for (idx_t k = 1; k <= 35 && i >= k; ++k)
                s += cache[i - k];
            seq_t ai = s * 21;              /* 21 * sum_{k=1..35} a[i-k] */
            if (i >= 36)                     /* subtract 231*a[i-36]   */
                ai -= cache[i - 36] * 231;

            /* add the inhomogeneous term b(i) */
            if (i == 0 || i == 36)
                ai += (seq_t)1;
            else if (i <= 35)
                ai += (seq_t)2;
            /* for i > 36, b(i) = 0 */

            cache[i] = ai;
        }
        len = new_len;
    }
    return cache[n];
}
